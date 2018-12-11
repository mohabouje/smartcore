#ifndef SMARTCORE_REVERBERATION_HPP
#define SMARTCORE_REVERBERATION_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Reverberation {
    public:

        enum Mode {
            Bidirectional,
            Omnidirectional,
            SubCardioid,
            Cardioid,
            HyperCardioid,
        };

        /**
         * Creates a Reverberation block with the given configuration
         * @param sample_rate Sampling frequency in Hz,
         * @param receiver_count  Number of receivers
         * @param sound_speed Speed of the sound
         * @param mode Operational mode.
         */
        Reverberation(std::int32_t sample_rate, std::uint8_t receiver_count, float sound_speed = 343.2f, Mode mode = Mode::SubCardioid);

        /**
         * @brief Default destructor
         */
        ~Reverberation();

        /**
         * @brief Returns the sound speed in m/sec.
         * @return Sound speed in m/sec.
         */
        float soundSpeed() const;

        /**
         * @brief Sets the sound velocity in m/sec.
         * @param speed Sound speed in m/sec.
         */
        void setSoundSpeed(float speed);

        /**
         * @brief Returns the position of the different receivers.
         * @return Array storing the position of each receiver.
         */
        const Vector<Point<float>>& receivers() const;
        
        /**
         * @brief Sets the position of the different receivers
         * @param positions Array storing the position of each receiver.
         */
        void setReceivers(const Vector<Point<float>>& positions);

        /**
         * @brief Returns the position of the source
         * @return Coordinate of the source
         */
        const Point<float>& source() const;

        /**
         * @brief Sets the position of the source
         * @param position Position of the source
         */
        void setSource(const Point<float>& position);

        /**
         * @brief Sets the room dimensions
         * @param width Width of the room
         * @param length Length of the room
         * @param height Height of the room
         */
        void setRoomDimension(float width, float length, float height);

        /**
         * @brief Returns the width of the room
         */
        float width() const;

        /**
         * @brief Returns the length of the room
         */
        float length() const;

        /**
         * @brief Returns the height of the room
         */
        float height() const;

        /**
         * @brief Returns the reverberation time (T60) in seconds.
         * @return Reverberation (T60) time in seconds.
         */
        float reverberationTime() const;

        /**
         * @brief Sets the reverberation time (T60)
         * @param t60 Reverberation time (T60) in seconds.
         */
        void setReverberationTime(float t60);

        /**
         * @brief Sets the reflection coefficients of the room.
         * @note Expected format: [beta_x1 beta_x2 beta_y1 beta_y2 beta_z1 beta_z2]
         */
        void setReflectionCoefficients(const Vector<float>& coefficients);

        /**
         * @brief Returns the operation mode of the Reverberation block.
         * @return Mode of operation.
         */
        Mode mode() const;

        /**
         * @brief Sets the operational mode of the Reverberation block.
         * @param mode Mode of operation
         */
        void setMode(Mode mode);

        /**
         * @brief Returns the maximum reflection order
         * @return Reflection order. (-1, maximum order)
         */
        int reflectionOrder() const;

        /**
         * @brief Sets the maximum reflection order.
         * @note Default: -1 (maximum)
         * @param order Maximum reflection order
         */
        void setReflectionOrder(int order);

        /**
         * @brief Sets the orientation of the receivers
         * @note Default: [0, 0]
         * @param azimuth Azimuth angle in radians.
         * @param elevation Elevation angle in radians.
         */
        void setReceiversOrientation(float azimuth, float elevation);

        /**
         * @brief Returns the computed RIR for each receiver
         * @return A matrix representing the RIR in each receiver.
         */
        const Matrix<double>& RIR() const;


        /**
         * @brief Adds the effect of the impulse response to input buffer and stored in the output buffer
         * @param recorded Input buffer
         * @param output Output buffer
         */
        void process(const AudioBuffer& input, AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };

}

#endif //SMARTCORE_REVERBERATION_HPP
