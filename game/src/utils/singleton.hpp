#pragma once

namespace aiko
{

    template <class T>
    class Singleton
    {
    public:

        static T& it()
        {
            static auto instance = std::make_unique<T>();
            return *instance;
        }

        Singleton(const Singleton&) = delete;
        Singleton& operator= (const Singleton) = delete;

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    };

    template <class T>
    class SingletonCtr
    {
    public:

        static T* it()
        {
            //assert(s_instance != nullptr);
            return s_instance;
        }

        // SingletonCtr(const SingletonCtr&) = delete;
        // SingletonCtr& operator= (const SingletonCtr) = delete;

    protected:
        SingletonCtr() = default;
        virtual ~SingletonCtr() = default;

        // Lazzy class just for this
        // Previous pointer create a copy if called in constructor
        static void setPtr(T* ptr)
        {
            s_instance = ptr;
        };

    private:

        static T* s_instance;

    };

    template < class T >
    T* SingletonCtr<T>::s_instance = nullptr;

}
