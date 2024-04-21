#pragma once
#include <vector>
#include <functional>

namespace EventSystem
{
    template<typename... T>
    class Event
    {
    public:
        using Listener = std::function<void(T...)>;

        void AddListener(Listener listener);
        void Trigger(T... args);

    private:
        std::vector<Listener> listeners;
    };
}
