#ifndef SMARTCORE_RECORDER_HPP
#define SMARTCORE_RECORDER_HPP

#include <audio_buffer.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace score {

    class Recorder {
    public:
        struct DeviceInfo {
            const char *name;
            int maxInputChannels;
            int maxOutputChannels;
            double defaultSampleRate;
        } ;

        /**
         * @brief Returns the default information about a device
         * @param index Index of the device
         * @return DeviceInfo struct holding some of the basic information about the device
         */
        static DeviceInfo DeviceInformation(std::int32_t index);

        /**
         * @brief Prints the information about all the available devices.
         */
        static void AvailableDevices();

        /**
         * @brief Returns the default input device index.
         * @return Device index
         */
        static int DefaultInputDevice();

        /**
         * @brief Initialize the internal dependencies.
         * This function should be called before any instantiation.
         */
        static void Initialize();

        /**
         * @brief Initialize a recorder in the default input device and with the default sample rate.
         * @param channels Number of channels of input signal.
         */
        explicit Recorder(std::int8_t channels);

        /**
         * @brief Initializes a recorder in the default input device with the given configuration.
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels of input signal.
         */
        Recorder(std::int32_t sample_rate, std::int8_t channels);

        /**
         * @brief Initializes a recorder with the given configuration
         * @param sample_rate Sampling rate in Hz.
         * @param channels Number of channels of input signal.
         * @param device_index Device Index
         * @param frames_per_buffer Number of frames per buffer.
         */
        Recorder(std::int32_t sample_rate, std::int8_t channels,
                int device_index = DefaultInputDevice(), std::size_t frames_per_buffer = 0);

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
        void setSampleRate(std::int32_t sample_rate);

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
        int deviceIndex() const;

        /**
         * @brief Updates the streaming's device
         * @param index Numeric value representing the device index.
         */
        void setDeviceIndex(int index);

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
        void setOnProcessingBufferReady(const std::function<void(AudioBuffer& buffer)>& callback);

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
