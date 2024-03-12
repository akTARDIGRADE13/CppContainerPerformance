#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

const int COUNT = 1000000;
const int ROOP = 1000;

void measureTimeArray(const int *randomAccess, const int size,
                      std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

        int data[size] = {};

        for(int i = 0; i < COUNT; ++i) {
            ++data[randomAccess[i]];
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        durations.push_back(duration.count());
    }
}

void measureTimeDynamicArrayC03(const int *randomAccess, const int size,
                                std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

        int *data = new int[size];
        std::fill(data, data + size, 0);

        for(int i = 0; i < COUNT; ++i) {
            ++data[randomAccess[i]];
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        durations.push_back(duration.count());
        delete[] data;
    }
}

void measureTimeDynamicArrayC11(const int *randomAccess, const int size,
                                std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

        std::unique_ptr<int[]> data = std::make_unique<int[]>(size);
        std::fill(data.get(), data.get() + size, 0);

        for(int i = 0; i < COUNT; ++i) {
            ++data[randomAccess[i]];
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        durations.push_back(duration.count());
    }
}

void measureTimeVectorArray(const int *randomAccess, const int size,
                            std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

        std::vector<int> data(size);

        for(int i = 0; i < COUNT; ++i) {
            ++data[randomAccess[i]];
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        durations.push_back(duration.count());
    }
}

template <typename T> long long calculateAverage(const std::vector<T> &values) {
    return std::accumulate(values.begin(), values.end(), 0) / values.size();
}

template <typename T> T calculateMedian(std::vector<T> &values) {
    std::sort(values.begin(), values.end());
    return values[ROOP / 2];
}

int main() {
    int randomAccess[COUNT];

    srand(static_cast<unsigned>(time(0)));

    int SIZE = 1000000 + rand() % 1000;

    // 配列にランダムな値をセット
    for(int i = 0; i < COUNT; ++i) {
        randomAccess[i] = rand() % SIZE; // 0からSIZE未満のランダムな整数
    }

    // 実行時間の計測
    std::vector<long long> durations;

    measureTimeArray(randomAccess, SIZE, durations);
    std::cout << "array - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeDynamicArrayC03(randomAccess, SIZE, durations);
    std::cout << "Dynamic Array (C++03) - Average: "
              << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeDynamicArrayC11(randomAccess, SIZE, durations);
    std::cout << "Dynamic Array (C++11) - Average: "
              << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeVectorArray(randomAccess, SIZE, durations);
    std::cout << "Vector - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    return 0;
}
