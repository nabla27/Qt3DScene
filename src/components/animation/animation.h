#ifndef ANIMATION_H
#define ANIMATION_H

#include <QAbstractAnimation>
#include <QThread>



class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent) : QObject(parent) {}

public:
    virtual QWidget* paramWidgets(QWidget *parent) const = 0;

public slots:
    virtual void update(const int& msec) = 0;

signals:
    void updated(const int& msec);
};










class AbstractAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    explicit AbstractAnimation(QObject *parent);
    ~AbstractAnimation();

public:
    int duration() const override { return _duration; }

public slots:
    void setDuration(const int duration);
    void setController(AbstractController *controller);

protected:
    virtual void updateCurrentTime(int currentTime) override { emit updateRequested(currentTime); }

private:
    int _duration = 0;
    QThread controllerThread;
    AbstractController *controller;

signals:
    void durationChanged(const int duration);
    void updateRequested(const int& msec);
    void currentTimeChanged(const int msec);
};













#endif // ANIMATION_H
