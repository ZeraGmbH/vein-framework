#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include <QByteArray>
#include <memory>
#include <google/protobuf/message.h>

class XiQNetWrapper
{
public:
    virtual ~XiQNetWrapper() = default;
    virtual std::shared_ptr<google::protobuf::Message> byteArrayToProtobuf(const QByteArray &byteArray);
    virtual QByteArray protobufToByteArray(const google::protobuf::Message &protobufMessage);
};

#endif // PROTOWRAPPER_H
