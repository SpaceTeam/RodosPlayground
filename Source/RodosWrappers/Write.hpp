#pragma once

#include <rodos.h>

#include <etl/string.h>

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <type_traits>


namespace rpg
{
template<typename T>
concept Writable = requires(T t, const void * sendBuf, size_t len)
{
    {t.write(sendBuf, len)};
};

template<typename T>
concept ReadWritable = requires(T t, const void * sendBuf, size_t len, void * recBuf, size_t maxLen)
{
    {t.writeRead(sendBuf, len, recBuf, maxLen)};
};


template<typename T, size_t size>
inline auto WriteTo(Writable auto communicationInterface, std::span<T, size> message)
{
    size_t nSentBytes = 0;

    auto thing = std::as_bytes(message);

    while(nSentBytes < message.size())
    {
        nSentBytes +=
            communicationInterface.write(thing.data() + nSentBytes, thing.size() - nSentBytes);
    }
}

inline auto WriteTo(Writable auto communicationInterface, std::string_view message)
{
    size_t nSentBytes = 0;
    while(nSentBytes < message.size())
    {
        nSentBytes +=
            communicationInterface.write(message.data() + nSentBytes, message.size() - nSentBytes);
    }
}


template<size_t size>
inline auto WriteToReadFrom(ReadWritable auto communicationInterface,
                            std::string_view message,
                            etl::string<size> * answer)
{
    answer->initialize_free_space();
    auto nReceivedBytes = communicationInterface.writeRead(
        message.data(), message.size(), answer->data(), answer->capacity());
    answer->trim_to_terminator();

    return nReceivedBytes;
}

}
