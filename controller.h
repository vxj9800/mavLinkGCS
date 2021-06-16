#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <linux/joystick.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString loc MEMBER loc NOTIFY locChanged)
    Q_PROPERTY(bool locValid READ getLocValid NOTIFY locValidChanged)
    Q_PROPERTY(float yaw MEMBER yaw NOTIFY yawChanged)
    Q_PROPERTY(float roll MEMBER roll NOTIFY rollChanged)
    Q_PROPERTY(float pitch MEMBER pitch NOTIFY pitchChanged)
    Q_PROPERTY(float throtl MEMBER throtl NOTIFY throtlChanged)

    QString loc;
    bool locValid = false;
    int fdescriptor = -1;
    QTimer dataReady;
    QFuture<void> future;

//    QFuture<void> future;

private slots:
    void initController();
    void emitSignals();

public:
    float yaw = 0, roll = 0, pitch = 0, throtl = 0;

    explicit controller(QObject *parent = nullptr);
    bool getLocValid(){return locValid;};
    void processEvent();
    ~controller(){locValid = false; close(fdescriptor);};

signals:
    void yawChanged();
    void rollChanged();
    void pitchChanged();
    void throtlChanged();
    void inputReady();
    void locChanged();
    void locValidChanged();

};

#endif // CONTROLLER_H
