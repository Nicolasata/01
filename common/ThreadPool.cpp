#include "./ThreadPool.hpp"

ThreadPool::~ThreadPool()
{
    for (int i = 0; i < (int)activeThreads.size(); i++){
        activeThreads[i].join();
    }
}

void ThreadPool::waitTask()
{
    for (;;){
        {
            std::unique_lock<std::mutex> uniqueLock(pMutex);
            pConvar.wait(uniqueLock, [this](){ return (!queue.empty()); } );

            if (!queue.empty()){
                try {
                    std::function<void()> callable = queue.front();
                    queue.pop();
                    callable();
                    //std::cout << "QUEUE SIZE: " << queue.size() << std::endl;
                } catch(...) {
                    std::cerr << "Caught exception: waitTask()" << std::endl;
                }
            }
        }
    }
}

void ThreadPool::createPool(int maxThread)
{
    for (int i = 0; i < maxThread; i++){
        activeThreads.push_back(std::thread(&ThreadPool::waitTask, this));
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(pMutex);
        //std::cout << "ENQUEUE" << std::endl;
        queue.push(task);
        pConvar.notify_one();
    }
}