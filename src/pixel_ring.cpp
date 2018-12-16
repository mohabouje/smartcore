#include "pixel_ring.hpp"
#include <libusb-1.0/libusb.h>
#include <pixel_ring.hpp>
#include <iostream>
#include <bitset>


using namespace score;




struct PixelRing::Pimpl {

    enum Commands : std::uint8_t {
        Trace = 0x00,
        Mono = 0x01,
        Listen = 0x02,
        Speak = 0x03,
        Think = 0x04,
        Spin = 0x05,
        Customize = 0x06,
        Brightness = 0x20,
        ColorPalette = 0x21,
        Central = 0x22,
        Volume = 0x23,
        Pattern = 0x024
    };

    Pimpl(uint16_t vid, uint16_t pid) {


        auto result = libusb_init(&ctx_);
        if (result < 0) {
            throw std::runtime_error("USB error (Initialization): " + std::string(libusb_error_name(result)));
        }

        libusb_set_debug(ctx_, 3);


        libusb_device **devices{nullptr};
        const auto number = libusb_get_device_list(ctx_, &devices);

        dev_handle_ = libusb_open_device_with_vid_pid(ctx_, vid, pid);
        if (!dev_handle_) {
            throw std::runtime_error("Could not open device");
        }

        libusb_free_device_list(devices, 1);
        if (libusb_kernel_driver_active(dev_handle_, 0) == 1) {
            if (auto r = libusb_detach_kernel_driver(dev_handle_, 0) != 0) {
                throw std::runtime_error("USB error (Device attached): " + std::string(libusb_error_name(r)));
            }
        }

        result = libusb_claim_interface(dev_handle_, 0);
        if (result < 0) {
            throw std::runtime_error("USB error (Claiming): " + std::string(libusb_error_name(result)));
        }

        unFlagAll();
    }

    ~Pimpl() {
        libusb_release_interface(dev_handle_, 0);
        libusb_close(dev_handle_);
        libusb_exit(ctx_);
    }

    void write(Commands cmd, std::uint8_t* data, std::uint16_t size = sizeof(std::uint8_t)) {
        constexpr auto timeout = 10;
        const auto result = libusb_control_transfer(dev_handle_,
                LIBUSB_TRANSFER_TYPE_CONTROL | LIBUSB_ENDPOINT_OUT |
                LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE,
                0, cmd, 0x1C, data, size, timeout);
        if (result < 0) {
            throw std::runtime_error("USB error (Writing): " + std::string(libusb_error_name(result)));
        }

    }

    void unFlagAll() {
        is_tracing_ = false;
        is_thinking_ = false;
        is_listening_ = false;
        is_speaking_ = false;
        is_spinning_ = false;

        for (auto& led : leds_) {
            led = 0;
        }
    }

    void trace() {
        if (is_tracing_) {
            return;
        }

        unFlagAll();
        is_tracing_ = true;
        write(Commands::Trace, &zero_);
    }

    void think() {
        if (is_thinking_) {
            return;
        }

        unFlagAll();
        is_thinking_ = true;
        write(Commands::Think, &zero_);
    }

    void listen() {
        if (is_listening_) {
            return;
        }

        unFlagAll();
        is_listening_ = true;
        write(Commands::Listen, &zero_);
    }

    void speak() {
        if (is_speaking_) {
            return;
        }

        unFlagAll();
        is_speaking_ = true;
        write(Commands::Speak, &zero_);
    }

    void spin() {
        if (is_spinning_) {
            return;
        }

        unFlagAll();
        is_spinning_ = true;
        write(Commands::Spin, &zero_);
    }


    std::uint32_t toColor(std::uint32_t red, std::uint32_t green, std::uint32_t blue) {
        return (red << 24) + (green << 16) + (blue << 8);
    }

    void turnOff() {
        setAll(0, 0, 0);
    }

    void turnOn() {
        setAll(255, 255, 255);
    }

    void setAll(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        auto color = toColor(r, g, b);
        for (auto& led : leds_) {
            led = color;
        }

        write(Commands::Mono, reinterpret_cast<std::uint8_t* >(&color), sizeof(color));
    }

    void setColor(std::uint8_t index, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        if (index >= NumberLED) {
            throw std::runtime_error("Maximum number of LED: " + std::to_string(NumberLED));
        }

        leds_[index] = toColor(r, g, b);
        write(Commands::Customize,
                reinterpret_cast<std::uint8_t* >(leds_.data()),
                sizeof(std::uint32_t) * leds_.size());
    }

    void setPattern(PixelRing::Pattern pattern) {
        std::uint8_t state = pattern;
        write(Commands::Pattern, &state);
    }

    void setVolume(std::uint8_t volume) {
        if (volume > NumberLED) {
            throw std::runtime_error("Maximum volume: " + std::to_string(volume));
        }
        write(Commands::Volume, &volume);
    }

    void setCentral(bool enabled) {
        Central_ = static_cast<uint8_t>(enabled);
        write(Commands::Central, &Central_);
    }

    void setPalette(std::uint8_t r1, std::uint8_t g1, std::uint8_t b1, std::uint8_t r2, std::uint8_t g2,
                    std::uint8_t b2) {
        palette_ = {{ toColor(r1, g1, b1), toColor(r2, g2, b2) }};
        write(Commands::ColorPalette,
              reinterpret_cast<std::uint8_t* >(palette_.data()),
              sizeof(std::uint32_t) * palette_.size());
    }

    void setBrightness(std::uint8_t brightness) {
        if (brightness > 0x1F) {
            throw std::runtime_error("Brightness range: [0, " + std::to_string(0x1F));
        }
        brightness_ = brightness;
        write(Commands::Brightness, &brightness_);
    }

    void setAngle(std::uint32_t angle, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        constexpr auto RangeLED = 360u / NumberLED;
        turnOff();
        setColor((angle % 360) / RangeLED, r, g, b);
    }

    bool is_tracing_{false};
    bool is_listening_{false};
    bool is_speaking_{false};
    bool is_thinking_{false};
    bool is_spinning_{false};

    std::uint8_t zero_{0};
    std::uint8_t Central_{0};
    std::uint8_t brightness_{};
    std::array<std::uint32_t, NumberLED> palette_{};
    std::array<std::uint32_t, NumberLED> leds_{toColor(255, 255, 255)};


    libusb_device_handle *dev_handle_{nullptr};
    libusb_context *ctx_{nullptr};
};

score::PixelRing::PixelRing(uint16_t vid, uint16_t pid) :
    pimpl_(std::make_unique<Pimpl>(vid, pid)) {

}

void score::PixelRing::trace() {
    pimpl_->trace();
}

void score::PixelRing::think() {
    pimpl_->think();
}

void score::PixelRing::listen() {
    pimpl_->listen();
}

void score::PixelRing::speak() {
    pimpl_->speak();
}

void score::PixelRing::spin() {
    pimpl_->spin();
}

void score::PixelRing::turnOff() {
    pimpl_->turnOff();
}

void score::PixelRing::turnOn() {
    pimpl_->turnOn();
}

void score::PixelRing::setAll(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    pimpl_->setAll(r, g, b);
}

void score::PixelRing::setColor(std::uint8_t index, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    pimpl_->setColor(index, r, g, b);
}

void score::PixelRing::setPattern(PixelRing::Pattern pattern) {
    pimpl_->setPattern(pattern);
}

void score::PixelRing::setVolume(std::uint8_t volume) {
    pimpl_->setVolume(volume);
}

void score::PixelRing::setCentralLED(bool enabled) {
    pimpl_->setCentral(enabled);
}

void score::PixelRing::setPalette(std::uint8_t r1, std::uint8_t g1, std::uint8_t b1, std::uint8_t r2, std::uint8_t g2,
                                  std::uint8_t b2) {
    pimpl_->setPalette(r1, g1, b1, r2, g2, b2);
}

void score::PixelRing::setBrightness(std::uint8_t brightness) {
    pimpl_->setBrightness(brightness);
}

void score::PixelRing::setAngle(std::uint32_t angle, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    pimpl_->setAngle(angle, r, g, b);
}

score::PixelRing::~PixelRing() = default;
