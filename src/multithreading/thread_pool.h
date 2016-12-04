#ifndef BSSRDF_MULTITHREADING_THREAD_POOL_H
#define BSSRDF_MULTITHREADING_THREAD_POOL_H

#include <future>
#include <thread>
#include <vector>

#include "multithreading/thread_safe_queue.h"

template <typename Task, typename Result>
class ThreadPool
{
public:
    using CallbackType = std::function<Result(Task)>;

    ThreadPool(std::size_t numOfThreads, const CallbackType& callback)
        : _numOfThreads(numOfThreads), _workerThreads(), _tasks(), _results(), _callback(callback)
    {
    }

    void start()
    {
        auto workerRun = std::bind(&ThreadPool::_workerThread, this, _callback);
        for (std::size_t i = 0; i < _numOfThreads; ++i)
        {
            std::thread t(workerRun);
            _workerThreads.push_back(std::move(t));
        }
    }

    void terminate()
    {
        _tasks.terminate();
        for (auto&& t : _workerThreads)
            t.join();
    }

    template <typename V>
    void enqueueTask(V&& task)
    {
        _tasks.enqueue(std::forward<V>(task));
    }

    Result dequeueResult()
    {
        return _results.dequeue();
    }

private:
    void _workerThread(const CallbackType& callback)
    {
        while (true)
        {
            auto task = _tasks.dequeue();
            if (_tasks.terminated())
                return;

            auto result = callback(task);
            _results.enqueue(std::move(result));
        }
    }

    std::size_t _numOfThreads;
    std::vector<std::thread> _workerThreads;
    ThreadSafeQueue<Task> _tasks;
    ThreadSafeQueue<Result> _results;
    CallbackType _callback;
};

#endif
