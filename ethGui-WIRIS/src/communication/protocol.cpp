#include "protocol.h"
#include <QStringList>
#include <sstream>

QString Protocol::delimiter = "";

QString Protocol::prepareMessage(const QString & command)
{
    return command + delimiter;
}

QString Protocol::prepareMessage(const QString & command, int arg)
{
    return command + " " + QString::number(arg) + delimiter;
}

QString Protocol::prepareMessage(const QString & command, bool arg)
{
    return command + " " + Protocol::boolToString(arg) + delimiter;
}

QString Protocol::prepareMessage(const QString & command, const QString & args)
{
    return command + " " + args + delimiter;
}

QString Protocol::prepareMessage(const QString & command, const QStringList & args)
{
    QString res = command;
    for(auto & arg: args)
        res += " " + arg;
    return res + delimiter;
}

QString Protocol::prepareMessageWithDefDelim(const QString &command)
{
    return command + "\n";
}

QString Protocol::prepareMessageWithDefDelim(const QString &command, const QString &args)
{
    return command + " " + args + "\n";
}


bool Protocol::isOk(const std::string & response)
{
    std::istringstream iss(response);
    std::string cache;
    getAnyLine(iss, cache);
    return cache == "OK";
}

bool Protocol::isOk(const QString & response){
    return Protocol::isOk(response.toStdString());
}

bool Protocol::isTrue(const std::string & response)
{
    std::istringstream iss(response);
    std::string cache;
    getAnyLine(iss, cache);
    return cache == "TRUE";
}

bool Protocol::isTrue(const QString & response){
    return Protocol::isTrue(response.toStdString());
}

bool Protocol::isPositive(const std::string & response)
{
    return Protocol::isOk(response) || Protocol::isTrue(response);
}

bool Protocol::isPositive(const QString & response){
    return Protocol::isOk(response) || Protocol::isTrue(response);
}

QString Protocol::boolToString(bool val)
{
    return val ? "TRUE" : "FALSE";
}

std::istream& Protocol::getAnyLine(std::istream& is, std::string& t)
{
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        case 0:
           is.setstate(std::ios::eofbit);
           return is;

        default:
            t += (char)c;
        }
    }
}
