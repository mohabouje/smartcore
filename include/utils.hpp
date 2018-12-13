#ifndef SMARTCORE_UTILS_HPP
#define SMARTCORE_UTILS_HPP

#include <cstdint>

namespace score {


    /**
     * The conversion functions use the following naming convention:
     * S16:      int16_t [-32768, 32767]
     * Float:    float   [-1.0, 1.0]
     * FloatS16: float   [-32768.0, 32767.0]
     * Dbfs: float [-20.0*log(10, 32768), 0] = [-90.3, 0]
     * The ratio conversion functions use this naming convention:
     * Ratio: float (0, +inf)
     * Db: float (-inf, +inf)
     */
    struct Converter {

        static void FloatToS16(const float* src, std::size_t size, int16_t* dest);

        static void S16ToFloat(const int16_t* src, std::size_t size, float* dest);

        static void FloatS16ToS16(const float* src, std::size_t size, int16_t* dest);

        static void S16ToFloatS16(const int16_t* src, std::size_t size, float* dest);

        static void FloatToFloatS16(const float* src, std::size_t size, float* dest);

        static  void FloatS16ToFloat(const float* src, std::size_t size, float* dest);

        static float FloatS16ToDbfs(float v);

    };



}

#endif //SMARTCORE_UTILS_HPP
