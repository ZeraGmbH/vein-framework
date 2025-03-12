#include "testcommandeventstrings.h"

using namespace VeinEvent;
using namespace VeinComponent;

QString TestCommandEventStrings::strSubtype(VeinEvent::CommandEvent::EventSubtype subtype)
{
    QString str;
    switch(subtype) {
    case CommandEvent::EventSubtype::NOTIFICATION:
        str = "NOTIFICATION";
        break;
    case CommandEvent::EventSubtype::TRANSACTION:
        str = "TRANSACTION";
        break;
    }
    return str;
}

QString TestCommandEventStrings::strOrigin(qint8 origin)
{
    QString str;
    switch(origin) {
    case qint8(EventData::EventOrigin::EO_LOCAL):
        str = "EO_LOCAL";
        break;
    case qint8(EventData::EventOrigin::EO_FOREIGN):
        str = "EO_FOREIGN";
        break;
    default:
        str = QString().arg("EO_USER_DEFINED + %1", origin - qint8(EventData::EventOrigin::EO_USER_DEFINED));
        break;
    }
    return str;
}

QString TestCommandEventStrings::strTarget(qint8 target)
{
    QString str;
    switch(target) {
    case qint8(EventData::EventTarget::ET_IRRELEVANT):
        str = "ET_IRRELEVANT";
        break;
    case qint8(EventData::EventTarget::ET_LOCAL):
        str = "ET_LOCAL";
        break;
    case qint8(EventData::EventTarget::ET_ALL):
        str = "ET_ALL";
        break;
    default:
        str = QString().arg("ET_USER_DEFINED + %1", target - qint8(EventData::EventTarget::ET_USER_DEFINED));
        break;
    }
    return str;
}

QString TestCommandEventStrings::strEntityCommand(VeinComponent::EntityData::Command cmd)
{
    QString str;
    switch(cmd) {
    case EntityData::Command::ECMD_INVALID:
        str = "ECMD_INVALID";
        break;
    case EntityData::Command::ECMD_ADD:
        str = "ECMD_ADD";
        break;
    case EntityData::Command::ECMD_REMOVE:
        str = "ECMD_REMOVE";
        break;
    case EntityData::Command::ECMD_SUBSCRIBE:
        str = "ECMD_SUBSCRIBE";
        break;
    case EntityData::Command::ECMD_UNSUBSCRIBE:
        str = "ECMD_UNSUBSCRIBE";
        break;
    }
    return str;
}

QString TestCommandEventStrings::strComponentCommand(VeinComponent::ComponentData::Command cmd)
{
    QString str;
    switch(cmd) {
    case ComponentData::Command::CCMD_INVALID:
        str = "CCMD_INVALID";
        break;
    case ComponentData::Command::CCMD_ADD:
        str = "CCMD_ADD";
        break;
    case ComponentData::Command::CCMD_REMOVE:
        str = "CCMD_REMOVE";
        break;
    case ComponentData::Command::CCMD_SET:
        str = "CCMD_SET";
        break;
    case ComponentData::Command::CCMD_FETCH:
        str = "CCMD_FETCH";
        break;
    }
    return str;
}

QString TestCommandEventStrings::strRPCCommand(VeinComponent::RemoteProcedureData::Command cmd)
{
    QString str;
    switch(cmd) {
    case RemoteProcedureData::Command::RPCMD_INVALID:
        str = "RPCMD_INVALID";
        break;
    case RemoteProcedureData::Command::RPCMD_CALL:
        str = "RPCMD_CALL";
        break;
    case RemoteProcedureData::Command::RPCMD_REGISTER:
        str = "RPCMD_REGISTER";
        break;
    case RemoteProcedureData::Command::RPCMD_RESULT:
        str = "RPCMD_RESULT";
        break;
    case RemoteProcedureData::Command::RPCMD_PROGRESS:
        str = "RPCMD_PROGRESS";
        break;
    case RemoteProcedureData::Command::RPCMD_CANCELLATION:
        str = "RPCMD_CANCELLATION";
        break;
    }
    return str;

}
