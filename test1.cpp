#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

const int SIZE = 1000000;
const int COUNT = 1000000;
const int ROOP = 1000;

int Array[SIZE];

std::array<int, SIZE> stdArray = {};
std::vector<int> vectorArray(SIZE);

template <typename T>
void measureTime(T &data, const int *randomAccess,
                 std::vector<long long> &durations) {
    for(int r = 0; r < ROOP; ++r) {
        auto start_time = std::chrono::high_resolution_clock::now();

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
    int *dynamicArrayC03 = new int[SIZE];
    std::unique_ptr<int[]> dynamicArrayC11 = std::make_unique<int[]>(SIZE);

    int randomAccess[COUNT];

    srand(static_cast<unsigned>(time(0)));

    // 配列にランダムな値をセット
    for(int i = 0; i < COUNT; ++i) {
        randomAccess[i] = rand() % SIZE; // 0からSIZE未満のランダムな整数
    }

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

    measureTime(Array, randomAccess, durations);
    std::cout << "array - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTime(stdArray, randomAccess, durations);
    std::cout << "std::array - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTime(dynamicArrayC03, randomAccess, durations);
    std::cout << "Dynamic Array (C++03) - Average: "
              << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTime(dynamicArrayC11, randomAccess, durations);
    std::cout << "Dynamic Array (C++11) - Average: "
              << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    durations.clear();
    measureTime(vectorArray, randomAccess, durations);
    std::cout << "Vector - Average: " << calculateAverage(durations)
              << " microseconds, Median: " << calculateMedian(durations)
              << " microseconds" << std::endl;

    // 動的配列のメモリ解放
    delete[] dynamicArrayC03;

    return 0;
}
