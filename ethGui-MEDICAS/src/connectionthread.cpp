#include "connectionthread.h"

std::shared_ptr<NetworkClient> ConnectionThread::networkClient() const
{
    return _networkClient;
}

ConnectionThread::ConnectionThread(std::shared_ptr<NetworkClient>networkClient, const QString& ip, const QString endpointPort)
    :  _networkClient(networkClient), _ip(ip), _endpointPort(endpointPort)
{ 
}

void ConnectionThread::run()
{
    emit connectionStarted();
    _networkClient = std::shared_ptr<NetworkClient>(new NetworkClient(_ip.toStdString(), _endpointPort.toStdString()));
    emit connectionFinished(_ip);
}
