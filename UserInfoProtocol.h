#pragma once

#include "ProtocolBase.h"
#include "ProtocolFactory.h"

PACK_STRUCT_BEGIN
struct UserInfo {
    uint32_t user_id;
    char username[32];
    uint16_t age;
}PACK_STRUCT_END;

class UserInfoProtocol : public ProtocolBase
{
public:
    static const uint16_t MSG_ID = 1001;

    UserInfoProtocol() {
        _header.msg_id = MSG_ID;
        _header.sequence = 0;
        _header.msg_len = sizeof(UserInfo);
    }

    virtual std::vector<uint8_t> Serialize() override;
    virtual bool Deserialize(const std::vector<uint8_t>& data) override;

    void SetUserInfo(const UserInfo& info) { user_info_ = info; }
    const UserInfo& GetUserInfo() const { return user_info_; }
public:
    UserInfo user_info_;
};

REGISTER_PROTOCOL(UserInfoProtocol);