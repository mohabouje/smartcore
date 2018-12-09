
#include <recorder.hpp>
#include <portaudio.h>

using namespace score;

struct Recorder::Pimpl {

    Pimpl(float sample_rate, std::size_t channels, std::size_t device_index, std::size_t frames_per_buffer) :
        device_index_(device_index),
        channels_(channels),
        frames_per_buffer_(static_cast<size_t>(frames_per_buffer ? frames_per_buffer : sample_rate / 100)),
        sample_rate_(sample_rate),
        array_(channels, std::vector<float>(frames_per_buffer, 0))
    {
        const auto err = Pa_Initialize();
        if (err != paNoError) {
            throw std::runtime_error(Pa_GetErrorText(err));
        }

        restart();
    }

    static int PortAudioCallback(const void *inputBuffer,
                                 void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo *timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData) {
        return ((Pimpl*)userData)->bufferReady(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
    }



    void setOnRecordingStarted(const std::function<void()>& callback) {
        on_recording_started_ = callback;
    }

    void setOnRecordingStopped(const std::function<void()>& callback) {
        on_recording_stopped_ = callback;
    }

    void setOnProcessingBufferReady(const std::function<void(const  std::vector<std::vector<float>>& array)>& callback) {
        on_buffer_ready_ = callback;
    }

    void restart() {
        input_params_.channelCount = channels_;
        input_params_.device = device_index_;
        input_params_.sampleFormat = paFloat32;
        input_params_.suggestedLatency = Pa_GetDeviceInfo(device_index_)->defaultLowInputLatency;
        input_params_.hostApiSpecificStreamInfo = NULL;

        const PaError err = Pa_OpenStream(&stream_, &input_params_, NULL, sample_rate_,
                frames_per_buffer_, paClipOff, &Pimpl::PortAudioCallback, this);

        if (err != paNoError) {
            throw std::runtime_error(Pa_GetErrorText(err));
        }
    }

    bool record() {
        if (isRunning()) {
            return true;
        }

        const PaError err = Pa_StartStream(stream_);
        if (err != paNoError) {
            throw std::runtime_error(Pa_GetErrorText(err));
        }

        const bool recording = isRunning();
        if (recording) {
            on_recording_started_();
        }
        return recording;
    }

    bool stop() {
        if (!isRunning()) {
            return true;
        }

        PaError err = Pa_StopStream(stream_);
        if ( err != paNoError ) {
            throw std::runtime_error(Pa_GetErrorText(err));
        }

        const bool stoped = !isRunning();
        if (stoped) {
            on_recording_stopped_();
        }
        return stoped;
    }


    int bufferReady(const void *inputBuffer,
                     void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags) {

        const auto* input = static_cast<const float*>(inputBuffer);
        for (auto i = 0ul; i < channels_; ++i) {
            for (auto j = 0ul, index = i; j < framesPerBuffer; ++j, index += channels_) {
                array_[i][j] = input[index];
            }
        }
        on_buffer_ready_(array_);
        return paContinue;
    }

    bool isSampleRateSupported(float sample_rate) {
        PaStreamParameters output_params;
        output_params.channelCount = 2;
        output_params.device = Pa_GetDefaultOutputDevice();
        output_params.sampleFormat = paFloat32;
        output_params.suggestedLatency = Pa_GetDeviceInfo(output_params.device)->defaultLowInputLatency;
        output_params.hostApiSpecificStreamInfo = NULL;
        return Pa_IsFormatSupported(&input_params_, &output_params, sample_rate);
    }

    void setSampleRate(float sample_rate) {
        if (!isSampleRateSupported(sample_rate)) {
            throw std::invalid_argument("Sample rate not supported.");
        }
        sample_rate_ = sample_rate;
    }


    void setDeviceIndex(std::size_t index) {
        if (index > Pa_GetDeviceCount()) {
            throw std::invalid_argument("Index out of bounds. Available decies: "
            + std::to_string(Pa_GetDeviceCount()));
        }

        if (Pa_GetDeviceInfo(index)->maxInputChannels <= 0) {
            throw std::runtime_error("Expected an input device. Device index: " + std::to_string(index));
        }

        device_index_ = index;
    }

    void setFramesPerBuffer(std::size_t frames_per_buffer) {
        frames_per_buffer_ = frames_per_buffer;
        array_ =  std::vector<std::vector<float>>(channels_, std::vector<float>(frames_per_buffer, 0));
    }

    bool isRunning() const {
        return Pa_IsStreamActive(stream_);
    }

    double timestamp() const {
        return Pa_GetStreamTime(stream_);
    }

    PaStream*               stream_{nullptr};
    PaStreamParameters      input_params_;

    std::size_t device_index_;
    std::size_t channels_;
    std::size_t frames_per_buffer_;
    float sample_rate_;


    std::vector<std::vector<float>> array_{};
    std::function<void()> on_recording_started_{nullptr};
    std::function<void()> on_recording_stopped_{nullptr};
    std::function<void(const  std::vector<std::vector<float>>& array)> on_buffer_ready_{nullptr};

};

Recorder::Recorder(float sample_rate, std::size_t channels,  std::size_t device_index = 0, std::size_t frames_per_buffer) :
    pimpl_(std::make_unique<Pimpl>(sample_rate, channels, device_index, frames_per_buffer)) {

}

Recorder::Recorder(float sample_rate, std::size_t channels) :
        Recorder(sample_rate, channels, Pa_GetDefaultInputDevice()){

}

Recorder::Recorder(std::size_t channels) :
        Recorder(Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->defaultSampleRate, channels, Pa_GetDefaultInputDevice()){

}

Recorder::~Recorder() = default;

void Recorder::reset() {
    pimpl_->restart();
}

void Recorder::setOnRecordingStarted(const std::function<void()> &callback) {
    pimpl_->setOnRecordingStarted(callback);
}

void Recorder::setOnRecordingStopped(const std::function<void()> &callback) {
    pimpl_->setOnRecordingStopped(callback);
}

void Recorder::setOnProcessingBufferReady(const std::function<void(const std::vector<std::vector<float>> &)> &callback) {
    pimpl_->setOnProcessingBufferReady(callback);
}

std::size_t Recorder::deviceIndex() const {
    return pimpl_->device_index_;
}

void Recorder::setDeviceIndex(std::size_t index) {
    pimpl_->setDeviceIndex(index);
}

void Recorder::setFramesPerBuffer(std::size_t number) {
    pimpl_->setFramesPerBuffer(number);
}

std::size_t Recorder::framesPerBuffer() const {
    return pimpl_->frames_per_buffer_;
}

float Recorder::sampleRate() const {
    return pimpl_->sample_rate_;
}

void Recorder::setSampleRate(float sample_rate) {
    pimpl_->setSampleRate(sample_rate);
}

bool Recorder::isRecording() {
    return pimpl_->isRunning();
}

bool Recorder::record() {
    return pimpl_->record();
}

bool Recorder::stop() {
    return pimpl_->stop();
}

float Recorder::timestamp() const {
    return static_cast<float>(pimpl_->timestamp());
}

std::string Recorder::deviceName() const {
    return  Pa_GetDeviceInfo(pimpl_->device_index_)->name;;
}

