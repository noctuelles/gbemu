//
// Created by plouvel on 12/10/24.
//

#ifndef UTILS_HXX
#define UTILS_HXX

#include <bitset>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>

#include "Common.hxx"

namespace utils
{
    template <typename E>
    concept EnumType = std::is_enum_v<E> && std::is_same_v<std::underlying_type_t<E>, uint8_t>;

    constexpr auto word_lsb(const uint16_t x)
    {
        return static_cast<uint8_t>(x & 0xFF);
    }

    constexpr auto word_msb(const uint16_t x)
    {
        return static_cast<uint8_t>((x & 0xFF00) >> 8);
    }

    constexpr auto to_word(const uint8_t msb, const uint8_t lsb)
    {
        return static_cast<uint16_t>(msb << 8 | lsb);
    }

    constexpr void to_bytes(const uint16_t word, uint8_t& msb, uint8_t& lsb)
    {
        msb = static_cast<uint8_t>(word >> 8);
        lsb = static_cast<uint8_t>(word & 0xFF);
    }

    constexpr bool address_in(const uint16_t addr, const MemoryMap::AddressRange& range)
    {
        return addr >= range.first && addr <= range.second;
    }

    template <EnumType Enum>
    constexpr std::underlying_type_t<Enum> operator&(const std::underlying_type_t<Enum> lhs, const Enum rhs)
    {
        return lhs & std::to_underlying(rhs);
    }

    template <EnumType Enum>
    constexpr std::underlying_type_t<Enum> operator|(const std::underlying_type_t<Enum> lhs, const Enum rhs)
    {
        return lhs | std::to_underlying(rhs);
    }

    template <typename T>
    class ThreadSafeQueue
    {
      public:
        void push(T value)
        {
            std::scoped_lock lock{m};
            q.push(value);
            cv.notify_one();
        }

        bool empty()
        {
            std::scoped_lock lock{m};
            return q.empty();
        }

        T pop()
        {
            std::unique_lock l{m};

            while (q.empty())
            {
                /* Release lock, pauses until one element is available in the queue. */
                cv.wait(l);
            }

            T value = std::move(q.front());
            q.pop();
            return value;
        }

        /**
         * Non-blocking pop.
         * @return T.
         */
        std::optional<T> try_pop()
        {
            std::scoped_lock l{m};
            if (q.empty())
            {
                return std::nullopt;
            }

            T value = std::move(q.front());
            q.pop();
            return value;
        }

      private:
        std::mutex              m{};
        std::queue<T>           q{};
        std::condition_variable cv{};
    };

}  // namespace utils

#endif  // UTILS_HXX
