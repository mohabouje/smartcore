#include <recorder.hpp>

using namespace score;
int main(int ac, char* av[]) {
    Recorder::Initialize();
    Recorder::AvailableDevices();
    return 0;
}