#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H


#include <qobject.h>
#include <qthread.h>
#include <QString>

#include <memory>
#include "networkclient.h"

class StreamThread : public QThread
{
        Q_OBJECT

public:
    StreamThread(bool* stream, const QString & ssrc) :  _ssrc(ssrc), _stream(stream)
    {}

signals:
    void imageSourceChanged();

protected:
    virtual void run() = 0;

    QString _ssrc;
    bool* _stream;
};


#endif // STREAMTHREAD_H
