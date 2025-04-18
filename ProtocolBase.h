#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>

#if defined(_MSC_VER)
#define PACK_STRUCT_BEGIN __pragma(pack(push, 1))
#define PACK_STRUCT_END   __pragma(pack(pop))
#else
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END    __attribute__((packed))
#endif

PACK_STRUCT_BEGIN
struct ProtocolHeader
{
    uint16_t msg_id;
    uint16_t msg_len;
    uint32_t sequence;
}PACK_STRUCT_END;

//#define ProtocolHeaderSize 8
constexpr int ProtocolHeaderSize = sizeof(ProtocolHeader);

// 平台相关的结构体打包宏定义
#if defined(_MSC_VER)
#define PACK_STRUCT_BEGIN __pragma(pack(push, 1))
#define PACK_STRUCT_END   __pragma(pack(pop))
#else
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END    __attribute__((packed))
#endif

// 字节序转换工具类
class ByteOrder {
public:
    static bool IsBigEndian() {
        constexpr uint32_t test = 0x01020304;
        return reinterpret_cast<const uint8_t*>(&test)[0] == 0x01;
    }

    template<typename T>
    static T SwapEndian(T value) {
        static_assert(std::is_integral<T>::value,
            "SwapEndian only works with integral types");
        union {
            T value;
            uint8_t bytes[sizeof(T)];
        } src, dst;
        src.value = value;
        for (size_t i = 0; i < sizeof(T); ++i) {
            dst.bytes[i] = src.bytes[sizeof(T) - 1 - i];
        }
        return dst.value;
    }

    template<typename T>
    static T NetworkToHost(T value) {
        if (IsBigEndian())
            return value;
        return SwapEndian(value);
    }

    template<typename T>
    static T HostToNetwork(T value) {
        if (IsBigEndian())
            return value;
        return SwapEndian(value);
    }
};

class ProtocolBase
{
public:
    virtual ~ProtocolBase() = default;

    virtual std::vector<uint8_t> Serialize() = 0;
    virtual bool Deserialize(const std::vector<uint8_t>& data) = 0;

    bool ParseHeader(const std::vector<uint8_t>& data)
    {
        if (data.size() < sizeof(ProtocolHeader))
            return false;

        memcpy(&_header, data.data(), sizeof(ProtocolHeader));
        _header.msg_id = ByteOrder::NetworkToHost(_header.msg_id);
        _header.msg_len = ByteOrder::NetworkToHost(_header.msg_len);
        _header.sequence = ByteOrder::NetworkToHost(_header.sequence);

        return true;
    }

    void SeirializeHeader(std::vector<uint8_t>& data)
    {
        ProtocolHeader net_header = _header;
        net_header.msg_id = ByteOrder::HostToNetwork(_header.msg_id);
        net_header.msg_len = ByteOrder::HostToNetwork(_header.msg_len);
        net_header.sequence = ByteOrder::HostToNetwork(_header.sequence);

        data.resize(sizeof(ProtocolHeader));
        memcpy(data.data(), &net_header, sizeof(ProtocolHeader));
    }

    void SetProtocolHeader(const ProtocolHeader& header) { _header = header; }
    const ProtocolHeader& GetProtocolHeader() { return _header; }
protected:
    ProtocolHeader _header;
};

