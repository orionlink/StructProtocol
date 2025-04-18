#pragma once

#include <memory>
#include <mutex>
#include <iostream>

template <class T>
class Singleton
{
public:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton<T>& operator=(const Singleton<T>& st) = delete;

    static std::shared_ptr<T> GetInstance() 
    {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = std::shared_ptr<T>(new T);
            });

        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
private:
    static std::shared_ptr<T> _instance;
};

template <class T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

