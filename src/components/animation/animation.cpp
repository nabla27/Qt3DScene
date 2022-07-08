#include "animation.h"

AbstractAnimation::AbstractAnimation(QObject *parent)
    : QAbstractAnimation(parent)
    , controller(nullptr)
{
    controllerThread.start();
}

AbstractAnimation::~AbstractAnimation()
{
    controllerThread.quit();
    controllerThread.wait();
}

void AbstractAnimation::setController(AbstractController *controller)
{
    if(controller)
    {
        if(this->controller) this->controller->deleteLater();
        this->controller = controller;

        controller->moveToThread(&controllerThread);

        connect(this, &AbstractAnimation::updateRequested, controller, &AbstractController::update);
        connect(controller, &AbstractController::updated, this, &AbstractAnimation::currentTimeChanged);
        connect(this, &AbstractAnimation::destroyed, controller, &AbstractController::deleteLater);
    }
}

void AbstractAnimation::setDuration(const int duration)
{
    if(_duration != duration)
    {
        _duration = duration;

        emit durationChanged(_duration);
    }
}
