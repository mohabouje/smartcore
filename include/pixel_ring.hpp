#ifndef SMARTCORE_PIXEL_RING_HPP
#define SMARTCORE_PIXEL_RING_HPP

#include <memory>

namespace score {

    class PixelRing {
    public:
        static constexpr auto NumberLED = 12u;

        enum Pattern {
            Google = 0,
            Echo
        };

        /**
         * @brief Connects to the PixelRing with the given configuration
         * @param vid Vendor ID
         * @param pid Product ID
         */
        explicit PixelRing(std::uint16_t vid = 0x2886, std::uint16_t pid = 0x0018);

        /**
         * @brief Default destructor
         */
        ~PixelRing();

        /**
         * @brief Puts the device in trace mode
         */
        void trace();

        /**
         * @brief Puts the device in think mode
         */
        void think();

        /**
         * @brief Puts the device in listen mode
         */
        void listen();

        /**
         * @brief Puts the device in speak mode
         */
        void speak();

        /**
         * @brief Puts the device in spin mode
         */
        void spin();

        /**
         * @brig Turns off all the available LEDs.
         */
        void turnOff();

        /**
         * @brief Turns on all the available LEDs. By default: white color.
         */
        void turnOn();

        /**
         * @brief Changes the color of all the LEDs
         * @param r Red component
         * @param g Green component
         * @param b Blue component
         */
        void setAll(std::uint8_t r, std::uint8_t g, std::uint8_t b);

        /**
         * @brief Changes the color of one LED
         * @param index Index of the LED [0, NumberLED[
         * @param r Red component
         * @param g Green component
         * @param b Blue component
         */
        void setColor(std::uint8_t index, std::uint8_t r, std::uint8_t g, std::uint8_t b);

        /**
         * @brief Changes the default pattern
         * @param pattern Pattern to be enabled
         */
        void setPattern(PixelRing::Pattern pattern);

        /**
         * @brief Displays the volume during a short period of time.
         * @note The colors are not changed. The level of volume is represented
         * by a reduction in the brightness of the LEDs from [0, volume]
         * @param volume Current volume
         */
        void setVolume(std::uint8_t volume);

        /**
         * @brief Changes the state of the central LED
         * @note This led can be used as control flag: the VAD is a good candidate.
         * @param enabled State of the LED.
         */
        void setCentralLED(bool enabled);

        /**
         * Changes the palette of colors
         * @param r1 Red component of the first color
         * @param g1 Green component of the first color
         * @param b1 Blue component of the first color
         * @param r2 Red component of the last color
         * @param g2 Green component of the last color
         * @param b2 Blue component of the last color
         */
        void setPalette(std::uint8_t r1, std::uint8_t g1, std::uint8_t b1,
                        std::uint8_t r2, std::uint8_t g2, std::uint8_t b2);

        /**
         * @brief Changes the brightness of the LEDs
         * @param brightness Brightness of the LEDs. Range: 0x00~0x1F
         */
        void setBrightness(std::uint8_t brightness);


        /**
         * @brief Enables the led at the given angle with the given color
         * @param angle Angle in degrees.
         * @param r Red component
         * @param g Green component
         * @param b Blue component
         */
        void setAngle(std::uint32_t angle, std::uint8_t r, std::uint8_t g, std::uint8_t b);

    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl_;
    };


}

#endif //SMARTCORE_PIXEL_RING_HPP
