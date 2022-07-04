#ifndef ANIMATION_H
#define ANIMATION_H

#include <QAbstractAnimation>
#include <QDebug> //DEBUG

class AbstractAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    explicit AbstractAnimation(QObject *parent) : QAbstractAnimation(parent) {}

    int duration() const override { return _duration; }

public slots:
    void setDuration(const int duration)
    {
        _duration = duration;
        emit durationChanged(duration);
    }

protected:
    virtual void updateCurrentTime(int currentTime) override { emit currentTimeChanged(currentTime); }

private:
    int _duration = 0;

signals:
    void durationChanged(const int duration);
    void currentTimeChanged(const int currentTime);
};













#endif // ANIMATION_H
