#include "reverberation.hpp"
#include "utils.hpp"

#include <edsp/spectral/convolution.hpp>

using namespace score;

#define _USE_MATH_DEFINES
#include <cmath>

#include <limits>
#include <cstdlib>
#include <iostream>
#include <complex>
#include <eigen3/unsupported/Eigen/FFT>

#define ROUND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


namespace internal {


    template<typename T> inline const T sinc(T const& x) {
        return (x==0) ? 1 : std::sin(x)/x;
    }

    // TODO: implement this thing correctly!
    double sim_microphone(double x, double y, double z, double* angle, Reverberation::Mode mtype) {
        if (mtype != Reverberation::Omnidirectional) {
            double gain, vartheta, varphi, rho;

            // Polar Pattern         rho
            // ---------------------------
            // Bidirectional         0
            // Hypercardioid         0.25
            // Cardioid              0.5
            // Subcardioid           0.75
            // Omnidirectional       1

            switch(mtype) {
            case Reverberation::Bidirectional:
                rho = 0;
                break;
            case Reverberation::HyperCardioid:
                rho = 0.25;
                break;
            case Reverberation::Cardioid:
                rho = 0.5;
                break;
            case Reverberation::SubCardioid:
                rho = 0.75;
                break;
            case Reverberation::Omnidirectional:
                rho = 1.0;
                break;
            };

            vartheta = std::acos(z / std::sqrt( x*x + y*y + z*z));
            varphi = std::atan2(y, x);

            gain = std::sin(M_PI/2-angle[1]) * sin(vartheta) * cos(angle[0]-varphi) + cos(M_PI/2-angle[1]) * cos(vartheta);
            gain = rho + (1-rho) * gain;

            return gain;
        } else {
            return 1;
        }
    }

    double gen_rir(Matrix<float>& imp,
                   double c,
                   double fs,
                   const Vector<Point<float>>& rr,
                   const Point<float>& ss,
                   const Point<float>& LL,
                   const Vector<float>& beta_input,
                   const std::pair<double, double>& orientation,
                   int isHighPassFilter,
                   int nDimension,
                   int nOrder,
                   int nSamples,
                   Reverberation::Mode microphone_type)
    {
        // | Room Impulse Response Generator                                  |\n"
        // |                                                                  |\n"
        // | Computes the response of an acoustic source to one or more       |\n"
        // | microphones in a reverberant room using the image method [1,2].  |\n"
        // |                                                                  |\n"
        // | Author    : dr.ir. Emanuel Habets (ehabets@dereverberation.org)  |\n"
        // |                                                                  |\n"
        // | Version   : 2.1.20141124                                         |\n"
        // |                                                                  |\n"
        // | Copyright (C) 2003-2014 E.A.P. Habets, The Netherlands.          |\n"
        // |                                                                  |\n"
        // | [1] J.B. Allen and D.A. Berkley,                                 |\n"
        // |     Image method for efficiently simulating small-room acoustics,|\n"
        // |     Journal Acoustic Society of America,                         |\n"
        // |     65(4), April 1979, p 943.                                    |\n"
        // |                                                                  |\n"
        // | [2] P.M. Peterson,                                               |\n"
        // |     Simulating the response of multiple microphones to a single  |\n"
        // |     acoustic source in a reverberant room, Journal Acoustic      |\n"
        // |     Society of America, 80(5), November 1986.                    |\n"
        // --------------------------------------------------------------------\n\n"
        // function [h, beta_hat] = rir_generator(c, fs, r, s, L, beta, nsample,\n"
        //  mtype, order, dim, orientation, hp_filter);\n\n"
        // Input parameters:\n"
        //  c           : sound velocity in m/s.\n"
        //  fs          : sampling frequency in Hz.\n"
        //  r           : M x 3 array specifying the (x,y,z) coordinates of the\n"
        //                receiver(s) in m.\n"
        //  s           : 1 x 3 vector specifying the (x,y,z) coordinates of the\n"
        //                source in m.\n"
        //  L           : 1 x 3 vector specifying the room dimensions (x,y,z) in m.\n"
        //  beta        : 1 x 6 vector specifying the reflection coefficients\n"
        //                [beta_x1 beta_x2 beta_y1 beta_y2 beta_z1 beta_z2] or\n"
        //                beta = reverberation time (T_60) in seconds.\n"
        //  nsample     : number of samples to calculate, default is T_60*fs.\n"
        //  mtype       : [omnidirectional, subcardioid, cardioid, hypercardioid,\n"
        //                bidirectional], default is omnidirectional.\n"
        //  order       : reflection order, default is -1, i.e. maximum order.\n"
        //  dim         : room dimension (2 or 3), default is 3.\n"
        //  orientation : direction in which the microphones are pointed, specified using\n"
        //                azimuth and elevation angles (in radians), default is [0 0].\n"
        //  hp_filter   : use 'false' to disable high-pass filter, the high-pass filter\n"
        //                is enabled by default.\n\n"
        // Output parameters:\n"
        //  h           : M x nsample matrix containing the calculated room impulse\n"
        //                response(s).\n"
        //  beta_hat    : In case a reverberation time is specified as an input parameter\n"
        //                the corresponding reflection coefficient is returned.\n\n");

        // Load parameters
        int          nMicrophones = rr.size();
        double       beta[6];
        double       angle[2];
        double       reverberation_time{0};

        if (beta_input.size() == 1) {
            double V = LL[0]*LL[1]*LL[2];
            double S = 2*(LL[0]*LL[2]+LL[1]*LL[2]+LL[0]*LL[1]);
            reverberation_time = beta_input[0];
            if (reverberation_time != 0) {
                double alfa = 24*V*log(10.0)/(c*S*reverberation_time);
                 if (alfa > 1)
                 	throw std::invalid_argument("Error: The reflection coefficients cannot be calculated using the current "
                 	             "room parameters, i.e. room size and reverberation time.\n           Please "
                 	             "specify the reflection coefficients or change the room parameters.");
                for (int i=0;i<6;i++)
                    beta[i] = sqrt(1-alfa);
            } else {
                for (int i=0;i<6;i++)
                    beta[i] = 0;
            }
        } else {
            for (int i=0;i<6;i++)
                beta[i] = beta_input[i];
        }

        // 3D Microphone orientation (optional)
        angle[0] = orientation.first;
        angle[1] = orientation.second;

        // Room Dimension (optional)
        if (nDimension == 2) {
            beta[4] = 0;
            beta[5] = 0;
        }

        // Reflection order (optional)
        if (nOrder < -1) {
           throw std::invalid_argument("Invalid reflection order. Expected a positive interger or -1 (maximum)");
        }

        // Number of samples (optional)
        if (beta_input.size() > 1) {
            double V = LL[0]*LL[1]*LL[2];
            double alpha = ((1-pow(beta[0],2))+(1-pow(beta[1],2)))*LL[1]*LL[2] +
                           ((1-pow(beta[2],2))+(1-pow(beta[3],2)))*LL[0]*LL[2] +
                           ((1-pow(beta[4],2))+(1-pow(beta[5],2)))*LL[0]*LL[1];
            reverberation_time = 24*log(10.0)*V/(c*alpha);
            if (reverberation_time < 0.128)
                reverberation_time = 0.128;
        }

        if (nSamples == -1) {
            nSamples = (int) (reverberation_time * fs);
        }


        imp.resize(nMicrophones, nSamples);

        // Temporary variables and constants (high-pass filter)
        const double W = 2*M_PI*100/fs; // The cut-off frequency equals 100 Hz
        const double R1 = exp(-W);
        const double B1 = 2*R1*cos(W);
        const double B2 = -R1 * R1;
        const double A1 = -(1+R1);
        double       X0;

        // Temporary variables and constants (image-method)
        const double Fc = 1; // The cut-off frequency equals fs/2 - Fc is the normalized cut-off frequency.
        const int    Tw = 2 * ROUND(0.004*fs); // The width of the low-pass FIR equals 8 ms
        const double cTs = c/fs;
        std::vector<double> LPI(Tw);
        std::array<double, 3> Y{}, r{}, s{}, L{};

        double       Rm[3];
        double       Rp_plus_Rm[3];
        double       refl[3];
        double       fdist,dist;
        double       gain;
        int          startPosition;
        int          n1, n2, n3;
        int          q, j, k;
        int          mx, my, mz;
        int          n;

        s[0] = ss[0]/cTs; s[1] = ss[1]/cTs; s[2] = ss[2]/cTs;
        L[0] = LL[0]/cTs; L[1] = LL[1]/cTs; L[2] = LL[2]/cTs;

        for (int idxMicrophone = 0; idxMicrophone < nMicrophones ; idxMicrophone++)
        {
            // [x_1 x_2 ... x_N y_1 y_2 ... y_N z_1 z_2 ... z_N]
            r[0] = rr[idxMicrophone][0] / cTs;
            r[1] = rr[idxMicrophone][1] / cTs;
            r[2] = rr[idxMicrophone][2] / cTs;

            n1 = (int) ceil(nSamples/(2*L[0]));
            n2 = (int) ceil(nSamples/(2*L[1]));
            n3 = (int) ceil(nSamples/(2*L[2]));

            // Generate room impulse response
            for (mx = -n1 ; mx <= n1 ; mx++)
            {
                Rm[0] = 2*mx*L[0];

                for (my = -n2 ; my <= n2 ; my++)
                {
                    Rm[1] = 2*my*L[1];

                    for (mz = -n3 ; mz <= n3 ; mz++)
                    {
                        Rm[2] = 2*mz*L[2];

                        for (q = 0 ; q <= 1 ; q++)
                        {
                            Rp_plus_Rm[0] = (1-2*q)*s[0] - r[0] + Rm[0];
                            refl[0] = pow(beta[0], std::abs(mx-q)) * pow(beta[1], std::abs(mx));

                            for (j = 0 ; j <= 1 ; j++)
                            {
                                Rp_plus_Rm[1] = (1-2*j)*s[1] - r[1] + Rm[1];
                                refl[1] = pow(beta[2], std::abs(my-j)) * pow(beta[3], std::abs(my));

                                for (k = 0 ; k <= 1 ; k++)
                                {
                                    Rp_plus_Rm[2] = (1-2*k)*s[2] - r[2] + Rm[2];
                                    refl[2] = pow(beta[4],std::abs(mz-k)) * pow(beta[5], std::abs(mz));

                                    dist = sqrt(pow(Rp_plus_Rm[0], 2) + pow(Rp_plus_Rm[1], 2) + pow(Rp_plus_Rm[2], 2));

                                    if (std::abs(2*mx-q)+std::abs(2*my-j)+std::abs(2*mz-k) <= nOrder || nOrder == -1)
                                    {
                                        fdist = floor(dist);
                                        if (fdist < nSamples)
                                        {
                                            gain = sim_microphone(Rp_plus_Rm[0], Rp_plus_Rm[1], Rp_plus_Rm[2], angle, microphone_type)
                                                   * refl[0]*refl[1]*refl[2]/(4*M_PI*dist*cTs);

                                            for (n = 0 ; n < Tw ; n++)
                                                LPI[n] =  0.5 * (1 - cos(2*M_PI*((n+1-(dist-fdist))/Tw))) * Fc * sinc(M_PI*Fc*(n+1-(dist-fdist)-(Tw/2)));

                                            startPosition = (int) fdist-(Tw/2)+1;
                                            for (n = 0 ; n < Tw; n++)
                                                if (startPosition+n >= 0 && startPosition+n < nSamples)
                                                    imp(idxMicrophone,startPosition + n) += gain * LPI[n];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // 'Original' high-pass filter as proposed by Allen and Berkley.
            if (isHighPassFilter == 1)
            {
                for (int idx = 0 ; idx < 3 ; idx++) {Y[idx] = 0;}
                for (int idx = 0 ; idx < nSamples ; idx++)
                {
                    X0 = imp(idxMicrophone, idx);
                    Y[2] = Y[1];
                    Y[1] = Y[0];
                    Y[0] = B1*Y[1] + B2*Y[2] + X0;
                    imp(idxMicrophone, idx) = Y[0] + A1*Y[1] + R1*Y[2];
                }
            }
        }
        return reverberation_time;
    }

}


struct Reverberation::Pimpl {

    Pimpl(std::int32_t sample_rate, std::uint8_t receiver_count, std::size_t frame_size,
          float sound_speed, Reverberation::Mode mode) :
            sample_rate_(sample_rate),
            receivers_count_(receiver_count),
            mode_(mode),
            sound_speed_(sound_speed),
            rir_size_(frame_size) {
    }

    void setReceivers(Vector<Point<float>> receivers) {
        if (receivers.size() != receivers_count_) {
            throw std::invalid_argument("The RIR generator is configured to work with "
                                        + std::to_string(receivers_count_) + " receivers.");
        }
        receivers_ = receivers;
    }

    void updateNeeded() {
        updated_need_ = true;
    }
    
    void setReverberationTime(float t60) {
        reverberation_time_ = t60;
        coefficients_ = {t60};
        updateNeeded();
    }

    void setReflectionCoefficients(const Vector<float> &coefficients) {
        if (coefficients.size() != 6) {
            throw std::invalid_argument("Expected time: [beta_x1 beta_x2 beta_y1 beta_y2 beta_z1 beta_z2]");
        }
        coefficients_ = coefficients;
        updateNeeded();
    }


    void process(const AudioBuffer &input, AudioBuffer &output) {

        if (input.sampleRate() != sample_rate_) {
            throw std::invalid_argument("Discrepancy in sampling rate. Expected "
                                        + std::to_string(sample_rate_) + " Hz");
        }

        if (receivers_count_ != input.channels()) {
            throw std::invalid_argument("The Reverberation is configure to work with "
                                        + std::to_string(receivers_count_) + " channels.");
        }

        if (receivers_count_ != receivers_.size()) {
            throw std::invalid_argument("Receivers positions not initialized.");
        }

        if (updated_need_) {
            reverberation_time_ = internal::gen_rir(rir_, sound_speed_, sample_rate_, receivers_, source_, {width_, length_, height_},
                              coefficients_, {azimuth_, elevation_}, true, 3, reflection_order_, rir_size_, mode_);
            updated_need_ = false;
        }

        output.setSampleRate(input.sampleRate());
        output.resize(input.channels(), input.framesPerChannel());
        for (auto i = 0ul; i < receivers_count_; ++i) {
            edsp::spectral::conv(input.channel(i),
                                 input.channel(i) + input.framesPerChannel(),
                                 static_cast<const float*>(rir_.row(i).data()),  output.channel(i));
        }
    }

    std::int8_t receivers_count_{0};
    std::int32_t sample_rate_{0};
    double reverberation_time_{0};
    float sound_speed_{0};
    float azimuth_{0};
    float elevation_{0};
    float height_{0};
    float width_{0};
    float length_{0};
    int reflection_order_{-1};
    int rir_size_{-1};
    Vector<float> coefficients_{0};
    Vector<Point<float>> receivers_{};
    Point<float> source_{};
    Mode mode_{};
    Matrix<float> rir_{};
    bool updated_need_{false};
};

Reverberation::Reverberation(std::int32_t sample_rate, std::uint8_t receiver_count, std::size_t frame_size, float sound_speed,
                             Reverberation::Mode mode) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, receiver_count, frame_size, sound_speed, mode)) {
}

void Reverberation::setReceiversOrientation(float azimuth, float elevation) {
    pimpl_->azimuth_ = azimuth;
    pimpl_->elevation_ = elevation;
    pimpl_->updateNeeded();
}

void Reverberation::setReflectionOrder(int order) {
    pimpl_->reflection_order_ = order;
    pimpl_->updateNeeded();
}

int Reverberation::reflectionOrder() const {
    return pimpl_->reflection_order_;
}

void Reverberation::setMode(Reverberation::Mode mode) {
    pimpl_->mode_ = mode;
    pimpl_->updateNeeded();
}

Reverberation::Mode Reverberation::mode() const {
    return pimpl_->mode_;
}

void Reverberation::setReverberationTime(float t60) {
    pimpl_->setReverberationTime(t60);
}

float Reverberation::reverberationTime() const {
    return pimpl_->reverberation_time_;
}

float Reverberation::height() const {
    return pimpl_->height_;
}

float Reverberation::length() const {
    return pimpl_->length_;
}

float Reverberation::width() const {
    return pimpl_->width_;
}

void Reverberation::setRoomDimension(float width, float length, float height) {
    pimpl_->width_ = width;
    pimpl_->length_ = length;
    pimpl_->height_ = height;
    pimpl_->updateNeeded();
}

void Reverberation::setSource(const Point<float> &position) {
    pimpl_->source_ = position;
    pimpl_->updateNeeded();
}

const Point<float> &Reverberation::source() const {
    return pimpl_->source_;
}

void Reverberation::setReceivers(const Vector<Point<float>> &positions) {
    pimpl_->setReceivers(positions);
}

const Vector<Point<float>> &Reverberation::receivers() const {
    return pimpl_->receivers_;
}

void Reverberation::setSoundSpeed(float speed) {
    pimpl_->sound_speed_ = speed;
    pimpl_->updateNeeded();
}

float Reverberation::soundSpeed() const {
    return pimpl_->sound_speed_;
}

const Matrix<float>& Reverberation::RIR() const {
    return pimpl_->rir_;
}

void Reverberation::process(const AudioBuffer &recorded, AudioBuffer &output) {
    pimpl_->process(recorded, output);
}

void Reverberation::setReflectionCoefficients(const Vector<float> &coefficients) {
    pimpl_->setReflectionCoefficients(coefficients);
}

Reverberation::~Reverberation() = default;


