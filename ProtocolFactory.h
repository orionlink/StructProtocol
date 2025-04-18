#pragma once
#include "ProtocolBase.h"
#include "Singleton.h"
#include <unordered_map>
#include <functional>
#include <memory>

class ProtocolFactory : public Singleton<ProtocolFactory>
{
    friend class Singleton<ProtocolFactory>;
public:
    using ProtocolCreator = std::function<std::shared_ptr<ProtocolBase>()>;

    void RegisterProtocol(uint16_t msg_id, ProtocolCreator creator) 
    {
        protocol_creators_[msg_id] = creator;
    }

    std::shared_ptr<ProtocolBase> ParseMessage(const std::vector<uint8_t>& data);
private:
    ProtocolFactory() = default;  // 显式声明默认构造函数

    std::unordered_map<uint16_t, ProtocolCreator> protocol_creators_;

    // 声明为友元，允许ProtocolBase访问私有成员
    template<typename T>
    friend class ProtocolRegister;
};

template<typename T>
class ProtocolRegister
{
public:
    explicit ProtocolRegister(uint16_t msg_id) {
        ProtocolFactory::GetInstance()->RegisterProtocol(msg_id, [] {
            return std::make_shared<T>();
            });
    }
};

// 注册协议的宏定义
#define REGISTER_PROTOCOL(ProtocolClass) \
    static ProtocolRegister<ProtocolClass> g_##ProtocolClass##_register(ProtocolClass::MSG_ID)

