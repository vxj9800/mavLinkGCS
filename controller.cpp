#include "controller.h"

controller::controller(QObject *parent) : QObject(parent)
{
    connect(this,&controller::locChanged,this,&controller::initController);
    connect(&dataReady,&QTimer::timeout,this,&controller::emitSignals);
    dataReady.start(1.0/50.0*1000);
}

void controller::initController()
{
    close(fdescriptor); fdescriptor = -1;
    fdescriptor = open(loc.toStdString().c_str(), O_RDONLY | O_NONBLOCK);
    if (fdescriptor > 0)
    {
        locValid = true;
        future = QtConcurrent::run(&controller::processEvent,this);
    }
    else
        locValid = false;
}

void controller::emitSignals()
{
    emit throtlChanged();
    emit rollChanged();
    emit pitchChanged();
    emit yawChanged();
    emit inputReady();
}

void controller::processEvent()
{
    qDebug() << locValid;
    while (locValid)
    {
        js_event event;

        int bytes = read(fdescriptor, &event, sizeof(event));
        if (bytes > 0)
        {
            if (event.type == JS_EVENT_AXIS && event.number == 2 ) // For L2 analog
                throtl = event.value/32767.0;
            else if (event.type == JS_EVENT_AXIS && event.number == 0 ) // For LJx analog
                roll = event.value/32767.0;
            else if (event.type == JS_EVENT_AXIS && event.number == 1 ) // For LJy analog
                pitch = event.value/32767.0;
            else if (event.type == JS_EVENT_AXIS && event.number == 3 ) // For RJx analog
                yaw = event.value/32767.0;
        }
    }
}
