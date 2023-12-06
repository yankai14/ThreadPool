//
// Created by Yan Kai Lim on 6/12/23.
//

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H


#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stop{};

    void execute_task() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [this] {return stop || !tasks.empty(); });

                if (stop && tasks.empty()) return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    };
public:
    explicit ThreadPool(int numThreads) {
        workers.reserve(numThreads);
        for (int i=0; i<numThreads; i++) {
            workers.emplace_back([this] {execute_task();});
        }
    }

    ~ThreadPool() {
        stop = false;
        cv.notify_all();
        for (std::thread& worker: workers) {
            worker.join();
        }
    }

    template<typename F, typename... Args>
    auto enqueueTask(F&& func, Args&&... args) {
        using ReturnType = decltype(func(args...));
        auto task = make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<F>(func), std::forward<Args>(args)...)
        );

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task]() mutable { (*task)(); });
        }

        cv.notify_one();

        return task->get_future();
    }
};


#endif //THREADPOOL_THREADPOOL_H
