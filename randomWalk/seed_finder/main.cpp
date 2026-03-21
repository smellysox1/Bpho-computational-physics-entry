#include <random>
#include <cmath>
#include <iostream>

// as a fraction of rng.max(), which is 2^32 - 1
constexpr unsigned int angles[10] = {
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 6,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 6 * 5,
    0xFFFFFFFF / 4 * 3,
    0xFFFFFFFF / 3 * 2,
    0,
    0,
    0xFFFFFFFF / 3
};

std::mt19937 rng;

int main() {
    unsigned int minTotalError = rng.max();

    for (unsigned int seed = 0; seed < rng.max(); seed++) {
        rng.seed(seed);

        unsigned int totalError = 0;

        for (int i = 0; i < 10; i++) {
            unsigned int angle = rng();
            unsigned int correctAngle = angles[i];

            unsigned int diff;

            if (angle > correctAngle)
                diff = angle - correctAngle;
            else
                diff = correctAngle - angle;

            if (rng.max() - totalError > diff)
                totalError += diff;
            else {
                totalError = rng.max();
                break;
            }
        }

        if (totalError < minTotalError) {
            minTotalError = totalError;
            std::cout << "New minimum found for seed " << seed << ", with total error " << totalError << "\n";
        }
    }

    std::cout << "Done!";
}