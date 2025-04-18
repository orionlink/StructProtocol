#include "UserInfoProtocol.h"

std::vector<uint8_t> UserInfoProtocol::Serialize()
{
    std::vector<uint8_t> data;
    SeirializeHeader(data);

    // ���л���Ϣ��
    UserInfo net_user_info = user_info_;
    net_user_info.user_id = ByteOrder::HostToNetwork(user_info_.user_id);
    net_user_info.age = ByteOrder::HostToNetwork(user_info_.age);

    size_t old_size = data.size();
    data.resize(old_size + sizeof(UserInfo));
    memcpy(data.data() + old_size, &net_user_info, sizeof(UserInfo));

    return data;
}

bool UserInfoProtocol::Deserialize(const std::vector<uint8_t>& data)
{
    if (!ParseHeader(data)) {  // ʹ�û���Ľ���ͷ������
        return false;
    }

    if (data.size() < sizeof(ProtocolHeader) + sizeof(UserInfo)) {
        return false;
    }

    // �����л���Ϣ��
    memcpy(&user_info_, data.data() + sizeof(ProtocolHeader), sizeof(UserInfo));
    user_info_.user_id = ByteOrder::NetworkToHost(user_info_.user_id);
    user_info_.age = ByteOrder::NetworkToHost(user_info_.age);

    std::cout << "&user_info_: " << &user_info_ << std::endl;

    return true;
}
