#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

const int SIZE = 10000000;
const int ROOP = 1000;

int Array[SIZE];

std::array<int, SIZE> stdArray = {};
std::vector<int> vectorArray(SIZE);

template <typename T>
void measureTimeRangeBasedFor(T &data, std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

        for(auto &element : data) {
            ++element;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        durations.push_back(duration.count());
    }
}

template <typename T>
void measureTimeIndexBasedFor(T &data, std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

        for(int i = 0; i < SIZE; ++i) {
            ++data[i];
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
    int *dynamicArrayC03 = new int[SIZE];
    std::unique_ptr<int[]> dynamicArrayC11 = std::make_unique<int[]>(SIZE);

    // データの初期化
    for(int i = 0; i < SIZE; ++i) {
        Array[i] = i;
        stdArray[i] = i;
        dynamicArrayC03[i] = i;
        dynamicArrayC11[i] = i;
        vectorArray[i] = i;
    }

    // 実行時間の計測
    std::vector<long long> durations;

    measureTimeRangeBasedFor(Array, durations);
    std::cout << "array - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeRangeBasedFor(stdArray, durations);
    std::cout << "std::array - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeIndexBasedFor(dynamicArrayC03, durations);
    std::cout << "Dynamic Array (C++03) - Average: "
              << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeIndexBasedFor(dynamicArrayC11, durations);
    std::cout << "Dynamic Array (C++11) - Average: "
              << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTimeRangeBasedFor(vectorArray, durations);
    std::cout << "Vector - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    // 動的配列のメモリ解放
    delete[] dynamicArrayC03;

    return 0;
}
