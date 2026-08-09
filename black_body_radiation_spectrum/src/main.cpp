#include <rplusplus.hpp>

#include <filesystem>

int main() {
    rpp::runSourceFile((std::filesystem::current_path().parent_path() / "black_body_radiation_spectrum/R/plancksLaw.R").string(),
                        "C:/bphocompphyschallenge");

    rpp::addParameter("Temperature 1 (K)", "T1", 6000, 0, 10000);
    rpp::addParameter("Temperature 2 (K)", "T2", 5000, 0, 10000);
    rpp::addParameter("Temperature 3 (K)", "T3", 4000, 0, 10000);

    if (!rpp::createWindow("Black Body Radiation Spectrum", 720, 720)) {
        return 1;
    }

    while (!rpp::shouldClose()) {
        rpp::drawFrame();
    }

    return 0;
}