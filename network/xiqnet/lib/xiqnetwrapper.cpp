#include "xiqnetwrapper.h"
#include <netmessages.pb.h>

std::shared_ptr<google::protobuf::Message> XiQNetWrapper::byteArrayToProto(const QByteArray &byteArray)
{
    ProtobufMessage::NetMessage *intermediate = new ProtobufMessage::NetMessage();
    if(!intermediate->ParseFromArray(byteArray, byteArray.size())) {
        ProtobufMessage::NetMessage::ScpiCommand *cmd = intermediate->mutable_scpi();
        cmd->set_command(byteArray.data(), byteArray.size());
    }
    std::shared_ptr<google::protobuf::Message> proto {intermediate};
    return proto;
}

QByteArray XiQNetWrapper::protoToByteArray(const google::protobuf::Message &protobufMessage)
{
    return QByteArray(protobufMessage.SerializeAsString().c_str(), protobufMessage.ByteSizeLong());
}
