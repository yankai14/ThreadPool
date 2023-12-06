//
// Created by Yan Kai Lim on 6/12/23.
//
// Example usage:

#include "ThreadPool.h"
#include <iostream>
#include <type_traits>

int main() {
    ThreadPool threadPool(4);

    auto future1 = threadPool.enqueueTask([](int x, int y) { std::cout << x + y << std::endl; }, 10, 20);
    auto future2 = threadPool.enqueueTask([](const std::string& str) { std::cout << str <<  std::endl; }, "Task 2");
    auto future3 = threadPool.enqueueTask([](const std::string& str) { std::cout << str << std::endl; }, "Task 3");
    auto future4 = threadPool.enqueueTask([](const std::string& str) { std::cout << str << std::endl; }, "Task 4");
    auto future5 = threadPool.enqueueTask([](const std::string& str) { std::cout << str << std::endl; }, "Task 5");

    // Retrieve results if needed
    future1.get();
    future2.get();
    future3.get();
    future4.get();
    future5.get();

    return 0;
}