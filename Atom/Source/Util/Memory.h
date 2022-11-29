#pragma once

#include <memory>

namespace Atom
{
    template<typename T>
    using UniqueRef = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr UniqueRef<T> CreateUniquePointer(Args&& ... args)
    {
	    return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using SharedRef = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr SharedRef<T> CreateSharedPointer(Args&& ... args)
    {
	    return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using WeakRef = std::weak_ptr<T>;
}