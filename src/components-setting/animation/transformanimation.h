#ifndef TRANSFORMANIMATION_H
#define TRANSFORMANIMATION_H

#include "animation.h"
#include "src/components-setting/abstractcomponentssettingwidget.h"
#include "src/components-setting/animation/animationsetting.h"
#include <Qt3DCore/QTransform>
#include <QThread>


class TransformController : public QObject
{
    Q_OBJECT

    typedef void (*UpdateTransformFuncType)(float*);

public:
    explicit TransformController(QObject *parent) : QObject(parent) {}

public slots:
    void setUpdateFunc(UpdateTransformFuncType func) { updateTransformFunc = func; }
    void update(const QVector3D& pos, const QVector3D& rot, const QVector3D& scale, const int& msec);

private:
    UpdateTransformFuncType updateTransformFunc = nullptr;

signals:
    void updated(const QVector3D& pos, const QVector3D& rot, const QVector3D& scale, const int& msec);
};








class TransformAnimation : public AbstractAnimation
{
    Q_OBJECT
public:
    explicit TransformAnimation(Qt3DCore::QTransform *transform);
    ~TransformAnimation();

protected:
    void updateCurrentTime(int msec) override;
    void receiveUpdatedParam(const QVector3D& pos, const QVector3D& rot, const QVector3D& scale, const int& time);

private:
    Qt3DCore::QTransform *transform;
    TransformController *controller;
    QThread thread;

signals:
    void updateRequested(const QVector3D& pos, const QVector3D& rot, const QVector3D& scale, const int& msec);
};










class TransformAnimationSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit TransformAnimationSettingWidget(Qt3DCore::QTransform *const transform,
                                             TransformAnimation *aimation,
                                             QWidget *parent);

    AbstractComponentsSettingWidget *const clone() const;

private:
    TransformAnimation *animation;
    AnimationControllBar *controllBar;
};







#endif // TRANSFORMANIMATION_H
