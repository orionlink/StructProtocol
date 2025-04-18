// ProtocolFactory.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ProtocolFactory.h"
#include "UserInfoProtocol.h"

union A
{
    uint32_t value;
    uint8_t bytes[sizeof(uint32_t)];
}src, dst;

using namespace std;

int main()
{
    //std::cout << "Hello World!\n";

    //src.value = 100;
    //std::cout << "src.value: " << src.value << endl;
    //// 打印 src.bytes 的每个字节（10进制形式）
    //std::cout << "src.bytes (dec): ";
    //for (size_t i = 0; i < sizeof(uint32_t); ++i)
    //{
    //    std::cout << static_cast<uint32_t>(src.bytes[i]) << " ";
    //}
    //std::cout << std::endl;

    //std::cout << "dst.value: " << dst.value << endl;

    // 创建并序列化协议
    ProtocolHeader protocolheader;
    protocolheader.msg_id = 1001;
    protocolheader.msg_len = sizeof(ProtocolHeader) + sizeof(UserInfoProtocol);
    protocolheader.sequence = 10;

    auto protocol = std::make_shared<UserInfoProtocol>();
    UserInfo user_info = { 12345, "test_user", 25 };
    protocol->SetUserInfo(user_info);
    protocol->SetProtocolHeader(protocolheader);
    auto serialized_data = protocol->Serialize();

    std::cout << "UserInfoProtocol&: " << protocol << endl;

    //for (int i = 0; i < serialized_data.size(); ++i)
    //{
    //    std::cout << "serialized_data: " << serialized_data[i] << " ";
    //}

    std::cout << "\n";

    // 解析消息
    auto parsed_protocol = ProtocolFactory::GetInstance()->ParseMessage(serialized_data);

    if (auto user_protocol = std::dynamic_pointer_cast<UserInfoProtocol>(parsed_protocol)) 
    {
        std::cout << "new UserInfoProtocol&: " << user_protocol << endl;

        const ProtocolHeader& parsed_header = user_protocol->GetProtocolHeader();
        std::cout << "msg_id: " << parsed_header.msg_id << endl;
        std::cout << "msg_len: " << parsed_header.msg_len << endl;
        std::cout << "sequence: " << parsed_header.sequence << endl;

        const UserInfo& parsed_user_info = user_protocol->GetUserInfo();
        std::cout << "age: " << parsed_user_info.age << endl;
        std::cout << "username: " << parsed_user_info.username << endl;
        std::cout << "user_id: " << parsed_user_info.user_id << endl;
    }

    std::cout << "\n";

    return 0;
}
