#include "Event.h"

namespace EventSystem
{
    template class Event<>;

    template<typename... T>
    void Event<T...>::AddListener(Listener listener)
    {
        listeners.push_back(listener);
    }

    template<typename... T>
    void Event<T...>::Trigger(T... args)
    {
        for (auto& listener : listeners)
        {
            listener(args...);
        }
    }
}
