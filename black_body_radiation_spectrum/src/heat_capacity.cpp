#include <rplusplus.hpp>

#include <filesystem>
#include <thread>

const auto basePath = std::filesystem::current_path().parent_path();

void limitFPS();

int main() {
    rpp::runSourceFile(
        (basePath / "black_body_radiation_spectrum/R/heat_capacity.R").string(),
        "C:/bphocompphyschallenge"
    );

    if (!rpp::createWindow("Molar heat capacity", 720, 720))
        return 1;

    while (!rpp::shouldClose()) {
        rpp::drawFrame();

        limitFPS();
    }

    return 0;
}

void limitFPS() {
    // 60 frames per second, in nanoseconds
    static const unsigned int targetFrameLength = 16666667;

    static auto frameStartTime = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto diff = now - frameStartTime;

    frameStartTime = now;

    // prevent integer overflow
    if (diff.count() >= targetFrameLength)
        return;

    std::this_thread::sleep_for(std::chrono::nanoseconds(targetFrameLength) - diff);
}