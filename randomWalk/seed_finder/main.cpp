#include <random>
#include <cmath>
#include <iostream>
#include <ctime>
#include <chrono>

// as a fraction of rng.max(), which is 2^32 - 1
constexpr const unsigned int angles[19] = {
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 6,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 6 * 5,
    0xFFFFFFFF / 4 * 3,
    0xFFFFFFFF / 3 * 2,
    0,
    0,
    0xFFFFFFFF / 3,
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 6,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 6 * 5,
    0xFFFFFFFF / 4 * 3,
    0xFFFFFFFF / 3 * 2,
    0,
    0
};

constexpr const unsigned int reverseAngles[19] = {
    0xFFFFFFFF / 3,
    0,
    0,
    0xFFFFFFFF / 3 * 2,
    0xFFFFFFFF / 4 * 3,
    0xFFFFFFFF / 6 * 5,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 6,
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 3,
    0,
    0,
    0xFFFFFFFF / 3 * 2,
    0xFFFFFFFF / 4 * 3,
    0xFFFFFFFF / 6 * 5,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 2,
    0xFFFFFFFF / 6
};

std::mt19937 rng;

std::string pad(int n) {
    if (n < 10)
        return '0' + std::to_string(n);
    else
        return std::to_string(n);
}

int main() {
    time_t timestamp = time(&timestamp);
    struct tm datetime = *localtime(&timestamp);

    auto startTime = std::chrono::system_clock::now();

    std::cout << "Start time: " << pad(datetime.tm_hour) << ":" << pad(datetime.tm_min) << ":" << pad(datetime.tm_sec) << "\n";


    for (unsigned int seed = 0; seed < rng.max(); seed++) {
        rng.seed(seed);
    
        unsigned int minTotalError = rng.max();

        unsigned int randomAngles[10];
    
        for (int i = 0; i < 10; i++)
            randomAngles[i] = rng();

        // forwards angles
        for (int startingIndex = 0; startingIndex < 10; startingIndex++) {
            unsigned int totalError = 0;

            for (int i = 0; i < 10; i++) {
                unsigned int correctAngle = angles[i + startingIndex];
                unsigned int randomAngle = randomAngles[i];

                unsigned int diff;

                if (randomAngle > correctAngle)
                    diff = randomAngle - correctAngle;
                else
                    diff = correctAngle - randomAngle;

                if (diff > 0x7FFFFFFF)
                    diff = 0xFFFFFFFF - diff;
                
                totalError += diff;
                
                if (totalError > 0x7FFFFFFF)
                    break;
            }

            if (totalError < minTotalError)
                minTotalError = totalError;
        }

        // reverse angles
        for (int startingIndex = 0; startingIndex < 10; startingIndex++) {
            unsigned int totalError = 0;

            for (int i = 0; i < 10; i++) {
                unsigned int correctAngle = reverseAngles[i + startingIndex];
                unsigned int randomAngle = randomAngles[i];

                unsigned int diff;

                if (randomAngle > correctAngle)
                    diff = randomAngle - correctAngle;
                else
                    diff = correctAngle - randomAngle;

                if (diff > 0x7FFFFFFF)
                    diff = 0xFFFFFFFF - diff;
                
                totalError += diff;
                
                if (totalError > 0x7FFFFFFF)
                    break;
            }

            if (totalError < minTotalError)
                minTotalError = totalError;
        }

        if (minTotalError < 0x4FFFFFFF) {
            using namespace std::chrono;
            std::cout << "Potential seed found: " << seed << ", with total error " << minTotalError << "\n";
            std::cout << (double)seed * 100.0 / (double)rng.max() << "% complete.\n";
            time_point endTime = system_clock::now() + nanoseconds((uint64_t)(system_clock::now() - startTime).count() / (uint64_t)seed * (uint64_t)rng.max());
            std::cout << "Projected end time: " << endTime << "\n\n";
        }
    }

    std::cout << "Done! ";

    timestamp = time(&timestamp);
    datetime = *localtime(&timestamp);

    std::cout << "Finished at: " << pad(datetime.tm_hour) << ":" << pad(datetime.tm_min) << ":" << pad(datetime.tm_sec) << "\n";

    std::cin.get();
}