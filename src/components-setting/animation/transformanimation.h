#ifndef TRANSFORMANIMATION_H
#define TRANSFORMANIMATION_H

#include "animation.h"
#include "src/components-setting/animation/animationsetting.h"
#include <Qt3DCore/QTransform>
#include <QThread>
#include <QMutex>






class TransformDllController : public AbstractController
{
    Q_OBJECT

    typedef void (*UpdateTransformFuncType)(float*) ;

public:
    explicit TransformDllController(Qt3DCore::QTransform *transform, QObject *parent = nullptr);

public:
    QList<QWidget*> paramWidgets() const override;

public slots:
    void update(const int& msec) override;

private:
    Qt3DCore::QTransform *transform;
    UpdateTransformFuncType updateTransformFunc;
    QMutex mutex;
};











//class TransformAnimationSettingWidget : public AbstractAnimationSettingWidget
//{
//    Q_OBJECT
//public:
//    explicit TransformAnimationSettingWidget(Qt3DCore::QTransform *const transform,
//                                             AbstractAnimation *aimation,
//                                             QWidget *parent);
//
//    AbstractComponentsSettingWidget *const clone() const;
//
//private slots:
//    void onSetDataMenu();
//
//private:
//    void setupMenu();
//
//private:
//    QMenu *setDataMenu;
//    AbstractAnimation *animation;
//};







#endif // TRANSFORMANIMATION_H
