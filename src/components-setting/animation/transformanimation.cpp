#include "transformanimation.h"
#include <QVector3D>




#include <QLibrary>
TransformDllController::TransformDllController(Qt3DCore::QTransform *transform, QObject *parent)
    : AbstractController(parent)
    , transform(transform)
{
    QLibrary lib("E:/repos/TestDll/x64/Debug/TestDll.dll");
    updateTransformFunc = (UpdateTransformFuncType)lib.resolve("updateTransform");

    connect(transform, &Qt3DCore::QTransform::destroyed, this, &TransformDllController::deleteLater);
}









QList<QWidget*> TransformDllController::paramWidgets() const
{
    return QList<QWidget*>();
}

void TransformDllController::update(const int& msec)
{
    mutex.lock();

    float p[10] = { transform->translation().x(), transform->translation().y(), transform->translation().z(),
                    transform->rotationX(), transform->rotationY(), transform->rotationZ(),
                    transform->scale3D().x(), transform->scale3D().y(), transform->scale3D().z(),
                    (float)msec / 1000 };

    updateTransformFunc(p);

    transform->setTranslation(QVector3D(p[0], p[1], p[2]));
    transform->setRotationX(p[3]);
    transform->setRotationY(p[4]);
    transform->setRotationZ(p[5]);
    transform->setScale3D(QVector3D(p[6], p[7], p[8]));

    mutex.unlock();

    emit updated(msec);
}

















#include <QFormLayout>
//DEBUG
#include <QToolButton>
#include <QLineEdit>
//TransformAnimationSettingWidget::TransformAnimationSettingWidget(Qt3DCore::QTransform *const transform,
//                                                                 TransformAnimation *animation,
//                                                                 QWidget *parent)
//    : AbstractAnimationSettingWidget(transform, animation, "Transform Animation", parent, true)
//    , setDataMenu(new QMenu(contents))
//    , animation(animation)
//{
//    QWidget *setControllerWidget = new QWidget(contents);
//    QHBoxLayout *setControllerLayout = new QHBoxLayout(setControllerWidget);
//    QLineEdit *controllerNameEdit = new QLineEdit("Test.dll", setControllerWidget);
//    QToolButton *setControllerButton = new QToolButton(setControllerWidget);
//
//    setControllerWidget->setLayout(setControllerLayout);
//    setControllerLayout->addWidget(controllerNameEdit);
//    setControllerLayout->addWidget(setControllerButton);
//
//    addFormRow("Controller", setControllerWidget);
//
//    setControllerWidget->setContentsMargins(0, 0, 0, 0);
//    setControllerLayout->setContentsMargins(0, 0, 0, 0);
//    setControllerLayout->setSpacing(0);
//}
//
//AbstractComponentsSettingWidget *const TransformAnimationSettingWidget::clone() const
//{
//    return nullptr;
//}
//
//void TransformAnimationSettingWidget::onSetDataMenu()
//{
//    setDataMenu->exec(cursor().pos());
//}
//
//void TransformAnimationSettingWidget::setupMenu()
//{
//    setDataMenu->addAction("DLL file");
//    setDataMenu->addAction("TSETEEWT");
//    setDataMenu->addAction("seta");
//}
//
















