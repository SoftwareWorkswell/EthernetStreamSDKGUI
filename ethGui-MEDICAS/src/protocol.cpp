#include "protocol.h"

QString Protocol::prepareMessage(const QString & command)
{
    return command + delimiter;
}

QString Protocol::prepareMessage(const QString & command, const QString & args)
{
    return command + " " + args + delimiter;
}

QString Protocol::prepareMessageWithDefDelim(const QString &command)
{
    return command + "\n";
}

QString Protocol::prepareMessageWithDefDelim(const QString &command, const QString &args)
{
    return command + " " + args + "\n";
}
