#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include "xiqnet_global.h"
#include <QByteArray>

namespace google
{
  namespace protobuf
  {
    class Message;
  }
}


/**
 * @brief A conveniant wrapper class for custom protobuf based classes
 *
 * It has to be implemented to use the "user specific" protocol buffers
 *
 * Often user specific overrides are not necessary because our implentations
 * match most clients' needs.
 */
class XiQNetWrapper
{
public:
    virtual ~XiQNetWrapper() {}
    /**
     * @brief Parses a QByteArray to create a google::protobuf::Message
     * @param t_byteArray
     * @return
     */
    virtual std::shared_ptr<google::protobuf::Message> byteArrayToProtobuf(QByteArray t_byteArray);

    /**
    * @brief Serializes a protobuf message to QByteArray
    * @param t_protobufMessage
    * @return
    */
    virtual QByteArray protobufToByteArray(const google::protobuf::Message &t_protobufMessage);
};

#endif // PROTOWRAPPER_H
