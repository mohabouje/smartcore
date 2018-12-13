#ifndef SMARTCORE_DECODER_HPP
#define SMARTCORE_DECODER_HPP

#include <audio_buffer.hpp>
#include <memory>

namespace score {

    class Decoder {
    public:
        /**
         * @brief Decodes the audio from an audio file.
         * @param file Path of the audio file.
         */
        explicit Decoder(const std::string& file);

        /**
         * @brief Default destructor.
         */
        ~Decoder();


        /**
         * @brief Checks if the there is an audio file opened.
         * @return true if an audio file has been opened.
         */
        bool is_open();
        /**
         * @brief Returns the number of samples in the audio file.
         * @note The number of samples is proportional to the number of channels and frames.
         * @return Number of samples in the audio file.
         */
        std::size_t samples() const;

        /*
         * @brief Returns the number of frames in the audio file.
         *
         * Each sample frame of audio consists of a fixed number of samples (equal to the number of audio channels in
         * the track. For monaural data, a sample frame consists of one audio sample. For stereophonic data, a sample
         * frame consists of a stereo pair.
         *
         * @returns Number of frames in the audio file.
         */
        std::size_t framesPerChannel() const;

        /**
         * @brief Returns the number of channels in the audio file.
         *
         * The number of channels is a positive integer which specifies the number of interleaved audio channels
         * in the given audio track.
         * @return Number of channels in the audio file.
         */
        std::int8_t channels();

        /**
         * @brief Returns the duration of the audio file in seconds.
         * @return Duration of the audio file in seconds.
         */
        double duration() const;

        /**
         * @brief Returns the sampling rate of the audio file in Hz.
         * @return Sampling rate of the audio file in Hz.
         */
        double sampleRate();

        /**
         * @brief Checks if the audio file is seekable.
         * @return true if the audio file is seekable, false otherwise.
         */
        bool seekable() const;

        /**
         * @brief Updates the current frame position for a track in an audio file.
         * @param position Frame position in the audio track.
         * @returns On success, returns the frame position in the audio track. On failure,
         * return the value -1
         */
        std::size_t seek(std::size_t position);

        /**
         * @brief Returns the current frame position of the track in an audio file.
         * @returns On success, returns the frame position in the audio track. On failure,
         * return the value -1
         */
        std::size_t current() const;
        

        /**
         * @brief Decodes the audio in the file and stores it in the Audio Buffer
         * @param output Buffer storing theaudio samples.
         */
        void process(AudioBuffer& output);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };



}

#endif //SMARTCORE_DECODER_HPP
