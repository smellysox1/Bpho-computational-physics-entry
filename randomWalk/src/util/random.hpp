#include <random>
#include <chrono>

namespace random {
    std::mt19937 rng{ static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()) };

    float randomFloat() {
        static constexpr float x = rng.max() / 2;
        return rng() / x - 1;
    }

    unsigned int randomInt() {
        return rng();
    }
}