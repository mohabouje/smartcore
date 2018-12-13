#include "utils.hpp"
#include <audio_util.h>

using namespace score;

void Converter::FloatToS16(const float *src, std::size_t size, int16_t *dest) {
    webrtc::FloatToS16(src, size, dest);
}

void Converter::S16ToFloatS16(const int16_t *src, std::size_t size, float *dest) {
    std::copy(src, src + size, dest);
}

void Converter::S16ToFloat(const int16_t *src, std::size_t size, float *dest) {
    webrtc::S16ToFloat(src, size, dest);
}

void Converter::FloatS16ToS16(const float *src, std::size_t size, int16_t *dest) {
    webrtc::FloatS16ToS16(src, size, dest);
}

void Converter::FloatToFloatS16(const float *src, std::size_t size, float *dest) {
    webrtc::FloatToFloatS16(src, size, dest);
}

void Converter::FloatS16ToFloat(const float *src, std::size_t size, float *dest) {
    webrtc::FloatS16ToFloat(src, size, dest);
}

float Converter::FloatS16ToDbfs(float v) {

    // kMinDbfs is equal to -20.0 * log10(-limits_int16::min())
    static constexpr float kMinDbfs = -90.30899869919436f;
    if (v <= 1.0f) {
        return kMinDbfs;
    }
    // Equal to 20 * log10(v / (-limits_int16::min()))
    return 20.0f * std::log10(v) + kMinDbfs;
}
