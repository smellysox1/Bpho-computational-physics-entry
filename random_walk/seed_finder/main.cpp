#include <random>
#include <cmath>
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>
#include <string>

const unsigned int numThreads = 12; // set this to number of logical processors
constexpr const unsigned int seedsPerThread = 0xFFFFFFFF / numThreads;

const unsigned int threshold = 0x07FFFFFF;

// as a fraction of rng.max(), which is 2^32 - 1
constexpr const unsigned int angles[9] = {
    0xFFFFFFFF / 20 * 9,
    0xFFFFFFFF / 20 * 17,
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 20 * 13,
    0xFFFFFFFF / 20,
    0xFFFFFFFF / 20 * 9,
    0xFFFFFFFF / 20 * 17,
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 20 * 13
};

constexpr const unsigned int reverseAngles[9] = {
    0xFFFFFFFF / 20,
    0xFFFFFFFF / 20 * 13,
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 20 * 17,
    0xFFFFFFFF / 20 * 9,
    0xFFFFFFFF / 20,
    0xFFFFFFFF / 20 * 13,
    0xFFFFFFFF / 4,
    0xFFFFFFFF / 20 * 17
};

std::string pad(int n) {
    if (n < 10)
        return '0' + std::to_string(n);
    else
        return std::to_string(n);
}

std::chrono::time_point<std::chrono::system_clock> startTime;

void findSeeds(unsigned int rangeMin, unsigned int rangeMax) {
    std::mt19937 rng;

    for (unsigned int seed = rangeMin; seed < rangeMax; seed++) {
        rng.seed(seed);

        unsigned int minTotalError = rng.max();

        unsigned int randomAngles[5];

        for (int i = 0; i < 5; i++)
            randomAngles[i] = rng();

        // forwards angles
        for (int startingIndex = 0; startingIndex < 5; startingIndex++) {
            unsigned int totalError = 0;

            for (int i = 0; i < 5; i++) {
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
        for (int startingIndex = 0; startingIndex < 5; startingIndex++) {
            unsigned int totalError = 0;

            for (int i = 0; i < 5; i++) {
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

        if (minTotalError < threshold) {
            using namespace std::chrono;
            std::cout << "Potential seed found: " << seed << ", with total error " << minTotalError << "\n";
            std::cout << "Approximately " << (double)(seed - rangeMin) * 100.0 / (double)seedsPerThread << "% complete.\n";
            time_point endTime = startTime + nanoseconds((uint64_t)(system_clock::now() - startTime).count() / (uint64_t)(seed - rangeMin) * (uint64_t)seedsPerThread);
            std::cout << "Projected end time (rough estimate): " << endTime << "\n\n";
        }
    }
}

int main() {
    time_t timestamp = time(&timestamp);
    struct tm datetime = *localtime(&timestamp);

    startTime = std::chrono::system_clock::now();

    std::cout << "Start time: " << pad(datetime.tm_hour) << ':' << pad(datetime.tm_min) << ':' << pad(datetime.tm_sec) << "\n\n";

    std::vector<std::thread> threads;

    threads.reserve(numThreads);

    for (int i = 0; i < numThreads - 1; i++)
        threads.emplace_back(findSeeds, seedsPerThread * i, seedsPerThread * (i + 1));

    threads.emplace_back(findSeeds, seedsPerThread * (numThreads - 1), 0xFFFFFFFF);

    for (std::thread& thread : threads)
        thread.join();

    std::cout << "Done! ";

    timestamp = time(&timestamp);
    datetime = *localtime(&timestamp);

    std::cout << "Finished at: " << pad(datetime.tm_hour) << ":" << pad(datetime.tm_min) << ":" << pad(datetime.tm_sec) << "\n";

    std::cin.get();
}