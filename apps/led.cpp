#include <pixel_ring.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <thread>

using namespace score;
namespace po = boost::program_options;

int main(int ac, char* av[]) {

    std::uint16_t vid, pid;

    po::options_description desc("Recording options");
    desc.add_options()
            ("help, h", "Help message")
            ("vendor, v", po::value<std::uint16_t>(&vid)->default_value(0x2886), "Vendor ID")
            ("product, p", po::value<std::uint16_t >(&pid)->default_value(0x0018), "Product ID");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }
    po::notify(vm);

    auto pixel = std::make_unique<PixelRing>(vid, pid);
    while (true) {
        pixel->trace();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        pixel->think();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        pixel->listen();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        pixel->spin();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (auto i = 0ul; i < 360ul; i += 10ul) {
            pixel->setAngle(i, 255, 255, 255);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}
