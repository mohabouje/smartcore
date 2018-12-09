#ifndef SMARTCORE_RECORDER_HPP
#define SMARTCORE_RECORDER_HPP

#include <memory>
#include <vector>
#include <functional>

namespace score {

    class Recorder {
    public:

        /**
         * @brief Initialize a recorder in the default input device and with the defaul sample rate.
         * @param channels Number of channels of input signal.
         */
        explicit Recorder(std::size_t channels);

        /**
         * @brief Initializes a recorder in the default input device with the given configuration.
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels of input signal.
         */
        Recorder(float sample_rate, std::size_t channels);

        /**
         * @brief Initializes a recorder with the given configuration
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels of input signal.
         * @param device_index Device Index
         * @param frames_per_buffer Number of frames per buffer.
         */
        Recorder(float sample_rate, std::size_t channels, std::size_t device_index, std::size_t frames_per_buffer = 0);

        /**
         * @brief Default destructor
         */
        ~Recorder();

        /**
         * @brief Checks if the current stream is active.
         * @return True if the stream is active, false otherwise.
         */
        bool isRecording();

        /**
         * @brief Starts the stream.
         * @return True if the stream is active, false otherwise.
         * @see isRecording
         */
        bool record();

        /**
         * @brief Stops the current stream.
         * @return True if the stream has been stopped, false otherwise.
         */
        bool stop();

        /**
         * @brief Returns the current time in seconds for a stream according to the system clock.
         * @return The stream's current time in seconds, or 0 if an error occurred.
         */
        float timestamp() const;

        /**
         * @brief Returns the stream sample rate.
         * @return Sample rate in Hz.
         */
        float sampleRate() const;

        /**
         * @brief Sets the streaming sample rate.
         * @param sample_rate The sample rate in Hz.
         */
        void setSampleRate(float sample_rate);

        /**
         * @brief Returns the number of frames per buffer.
         * @return Number of frames per buffer.
         */
        std::size_t framesPerBuffer() const;

        /**
         * @brief Set the number of frames per buffer.
         * @param number Number of frames per buffer.
         */
        void setFramesPerBuffer(std::size_t number);

        /**
         * @brief Returns the associated name with the current device.
         * @return Name of the current decide.
         */
        std::string deviceName() const;

        /**
         * @brief Returns the current device index.
         * @return Numeric value representing the device index.
         */
        std::size_t deviceIndex() const;

        /**
         * @brief Updates the streaming's device
         * @param index Numeric value representing the device index.
         */
        void setDeviceIndex(std::size_t index);

        /**
         * @brief Updates the listener that may be called when the recording starts.
         * @param callback Callback to be called.
         */
        void setOnRecordingStarted(const std::function<void()>& callback);

        /**
         * @brief Updates the listener that may be called when the recording stops.
         * @param callback Callback to be called.
         */
        void setOnRecordingStopped(const std::function<void()>& callback);

        /**
         * @brief Updates the listener that may be called when a frame is ready to be processed.
         * @param callback Callback to be called.
         */
        void setOnProcessingBufferReady(const std::function<void(const  std::vector<std::vector<float>>& array)>& callback);

        /**
         * @brief Re-initializes the block, clearing all state.
         * @note This function should be called after any modification in the streaming.
         */
        void reset();

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };



}

#endif //SMARTCORE_RECORDER_HPP
