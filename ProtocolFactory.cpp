#include "ProtocolFactory.h"

std::shared_ptr<ProtocolBase> ProtocolFactory::ParseMessage(const std::vector<uint8_t>& data)
{
    if (data.size() < ProtocolHeaderSize)
    {
        return nullptr;
    }

    ProtocolHeader header;
    memcpy(&header, data.data(), ProtocolHeaderSize);
    uint16_t msg_id = ByteOrder::NetworkToHost(header.msg_id);

    auto iter = protocol_creators_.find(msg_id);
    if (iter == protocol_creators_.end())
        return nullptr;

    auto protocol = iter->second();
    if (protocol->Deserialize(data))
    {
        return protocol;
    }

    return nullptr;
}
