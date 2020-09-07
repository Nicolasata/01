#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

#include <queue>
#include <functional>
#include <condition_variable>

class ThreadPool
{
    private:

        std::condition_variable pConvar;
        std::mutex pMutex;

        std::vector<std::thread> activeThreads;
        std::queue<std::function<void()>> queue;

    public:

        void waitTask();
        void createPool(int maxThread);
        void enqueue(std::function<void()> task);

        ThreadPool(){};
        ~ThreadPool();
};

#endif