#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <QMutex>

#include <src/communication/networkclient.h>

class ConnectionThread : public QThread
{
    Q_OBJECT
    std::shared_ptr<NetworkClient> _networkClient = nullptr;
    QString _ip = "";
    QString _endpointPort = "";
public:
    ConnectionThread(std::shared_ptr<NetworkClient> networkClient, const QString& ip, const QString endpointPort = "2240");
    ConnectionThread(const QString& ip, const QString endpointPort);
    std::shared_ptr<NetworkClient> networkClient() const;

protected:
    virtual void run() override;
signals:
    void connectionFinished(const QString& ip);
    void connectionStarted();
};


#endif // CONNECTIONTHREAD_H
