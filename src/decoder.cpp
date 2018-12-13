#include "decoder.hpp"
#include <edsp/io/decoder.hpp>

using namespace score;

struct Decoder::Pimpl {

    Pimpl(const std::string& file)  {

        decoder_.open(file);

    }

    ~Pimpl() {
        decoder_.close();
    }


    void process(AudioBuffer &output, std::size_t size) {
        if (output.channels() != decoder_.channels()) {
            throw std::runtime_error("Expected an output buffer of "
                                     + std::to_string(decoder_.channels()) + " channels.");
        }

        interleave_.resize(size);
        decoder_.read(std::begin(interleave_), std::end(interleave_));

        output.setSampleRate(decoder_.samplerate());
        output.fromInterleave(decoder_.channels(), size, interleave_.data());
    }


    std::vector<float> interleave_;
    edsp::io::decoder<float> decoder_;
};

score::Decoder::Decoder(const std::string &file) : pimpl_(std::make_unique<Pimpl>(file)) {

}

score::Decoder::~Decoder() = default;

void score::Decoder::process(AudioBuffer &output, std::size_t frames) {
    pimpl_->process(output, frames);
}

bool score::Decoder::is_open() {
    return pimpl_->decoder_.is_open();
}

std::size_t score::Decoder::samples() const {
    return pimpl_->decoder_.samples();
}

std::size_t score::Decoder::framesPerChannel() const {
    return pimpl_->decoder_.frames();
}

std::int8_t score::Decoder::channels() {
    return pimpl_->decoder_.channels();
}

double score::Decoder::duration() const {
    return pimpl_->decoder_.duration();
}

double score::Decoder::sampleRate() {
    return pimpl_->decoder_.samplerate();
}

bool score::Decoder::seekable() const {
    return pimpl_->decoder_.seekable();
}

std::size_t score::Decoder::seek(std::size_t position) {
    return pimpl_->decoder_.seek(position);
}

std::size_t score::Decoder::current() const {
    return pimpl_->decoder_.current();
}
