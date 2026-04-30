#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include <QByteArray>
#include <memory>
#include <google/protobuf/message.h>

class XiQNetWrapper
{
public:
    static std::shared_ptr<google::protobuf::Message> byteArrayToProto(const QByteArray &byteArray);
    static QByteArray protoToByteArray(const google::protobuf::Message &protobufMessage);
};

#endif // PROTOWRAPPER_H
