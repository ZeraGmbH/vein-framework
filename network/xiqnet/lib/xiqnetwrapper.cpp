#include "xiqnetwrapper.h"
#include <netmessages.pb.h>

std::shared_ptr<google::protobuf::Message> XiQNetWrapper::byteArrayToProtobuf(QByteArray t_byteArray)
{
    ProtobufMessage::NetMessage *intermediate = new ProtobufMessage::NetMessage();
    if(!intermediate->ParseFromArray(t_byteArray, t_byteArray.size()))
    {
        ProtobufMessage::NetMessage::ScpiCommand *cmd = intermediate->mutable_scpi();
        cmd->set_command(t_byteArray.data(), t_byteArray.size());
    }
    std::shared_ptr<google::protobuf::Message> proto {intermediate};
    return proto;
}

QByteArray XiQNetWrapper::protobufToByteArray(const google::protobuf::Message &t_protobufMessage)
{
  return QByteArray(t_protobufMessage.SerializeAsString().c_str(), t_protobufMessage.ByteSizeLong());
}
