#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>

std::mutex sumMutex;
std::mutex minMutex;
std::mutex maxMutex;

int sum = 0;
int max = 0;
int min = 100;


void calculateSum(const std::vector<int>& vec) {
    for (int value : vec) {
        std::lock_guard<std::mutex> lock(sumMutex); // Захватываем мьютекс для summ
        sum += value;
    }
}

void findMax(const std::vector<int>& vec) {
    for (int value : vec) {
        std::lock_guard<std::mutex> lock(maxMutex); // Захватываем мьютекс для поиска макс
        if (value > max) {
            max = value;
        }
    }
}

void findMin(const std::vector<int>& vec) {
    for (int value : vec) {
        std::lock_guard<std::mutex> lock(minMutex); // Захватываем мьютекс для поиска мин
        if (value < min) {
            min = value;
        }
    }
}





int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);

    std::vector<int> vec(100);
    for (int& value : vec) {
        value = distrib(gen);
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    //  Запускаем потоки
    std::thread threadSum(calculateSum, std::ref(vec));
    std::thread threadMax(findMax, std::ref(vec));
    std::thread threadMin(findMin, std::ref(vec));

    //  Ждем завершения потоков
    threadSum.join();
    threadMax.join();
    threadMin.join();

    //  Выводим результаты
    std::cout << "Summ: " << sum << std::endl;
    std::cout << "Max elem: " << max << std::endl;
    std::cout << "Min elem: " << min << std::endl;

    return 0;
}
