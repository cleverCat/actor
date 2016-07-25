#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton:public T
{
public:
    Singleton<T>():T(){}
    Singleton<T>(const Singleton<T>& singleton);
    Singleton<T>& operator=(const Singleton<T>&);
public:
    static Singleton<T>& instance(void)
    {
        static Singleton<T> instance;
        return instance;
    }
};

#endif // SINGLETON_H
