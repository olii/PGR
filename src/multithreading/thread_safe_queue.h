#ifndef BSSRDF_MULTITHREADING_THREAD_SAFE_QUEUE_H
#define BSSRDF_MULTITHREADING_THREAD_SAFE_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
public:
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _impl.empty();
    }

    template <typename V>
    void enqueue(V&& value)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_terminate)
            return;

        _impl.push(std::forward<V>(value));
        _cv.notify_one();
    }

    T dequeue()
    {
        std::unique_lock<std::mutex> lock(_mutex);

        if (_terminate)
            return {};

        while (_impl.empty())
        {
            _cv.wait(lock);
            if (_terminate)
                return {};
        }

        T value = _impl.front();
        _impl.pop();
        return value;
    }

    void terminate()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _terminate = true;
        _cv.notify_all();
    }

    bool terminated()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _terminate;
    }

private:
    std::queue<T> _impl;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _terminate = false;
};

#endif
