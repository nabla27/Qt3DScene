#include "transformanimation.h"
#include <QVector3D>

//DEBUG
#include <QLibrary>
TransformController::TransformController(QObject *parent)
    : QObject(parent)
{
}

void TransformController::update(const QVector3D& pos,
                                 const QVector3D& rot,
                                 const QVector3D& scale,
                                 const int& msec)
{
    if(updateTransformFunc)
    {
        float p[10] =
        {
            pos.x(), pos.y(), pos.z(),
            rot.x(), rot.y(), rot.z(),
            scale.x(), scale.y(), scale.z(),
            float(msec) / 1000
        };

        updateTransformFunc(p);

        emit updated(QVector3D(p[0], p[1], p[2]), QVector3D(p[3], p[4], p[5]), QVector3D(p[6], p[7], p[8]), msec);
    }
}

void TransformController::setUpdateFunc(UpdateTransformFuncType func)
{
    updateTransformFunc = func;
}








TransformAnimation::TransformAnimation(Qt3DCore::QTransform *transform)
    : AbstractAnimation(transform)
    , transform(transform)
    , controller(new TransformController(nullptr))
{
    controller->moveToThread(&thread);
    thread.start();

    connect(this, &TransformAnimation::updateRequested, controller, &TransformController::update);
    connect(controller, &TransformController::updated, this, &TransformAnimation::receiveUpdatedParam);

    connect(&thread, &QThread::finished, controller, &TransformController::deleteLater);
}

TransformAnimation::~TransformAnimation()
{
    thread.quit();
    thread.wait();
}

void TransformAnimation::updateCurrentTime(int msec)
{
    emit updateRequested(transform->translation(), transform->rotation().vector(), transform->scale3D(), msec);
}

void TransformAnimation::receiveUpdatedParam(const QVector3D &pos, const QVector3D &rot, const QVector3D &scale, const int &msec)
{
    transform->setTranslation(pos);
    transform->setRotation(QQuaternion(1.0f, rot));
    transform->setScale3D(scale);

    emit currentTimeChanged(msec);
}










#include <QFormLayout>
//DEBUG
#include <QPushButton>
TransformAnimationSettingWidget::TransformAnimationSettingWidget(Qt3DCore::QTransform *const transform,
                                                                 TransformAnimation *animation,
                                                                 QWidget *parent)
    : AbstractComponentsSettingWidget(transform, "Transform Animation", parent, true)
    , setDataMenu(new QMenu(contents))
    , animation(animation)
    , controllBar(new AnimationControllBar(contents, animation))
{
    QVBoxLayout *vLayout = new QVBoxLayout(contents);
    QFormLayout *fLayout = new QFormLayout;

    contents->setLayout(vLayout);
    vLayout->addWidget(controllBar);
    vLayout->addLayout(fLayout);

    QPushButton *button = new QPushButton(contents);
    fLayout->addRow("Set data", button);

    setupMenu();

    connect(button, &QPushButton::released, this, &TransformAnimationSettingWidget::onSetDataMenu);
}

AbstractComponentsSettingWidget *const TransformAnimationSettingWidget::clone() const
{
    return nullptr;
}

void TransformAnimationSettingWidget::onSetDataMenu()
{
    setDataMenu->exec(cursor().pos());
}

void TransformAnimationSettingWidget::setupMenu()
{
    setDataMenu->addAction("DLL file");
    setDataMenu->addAction("TSETEEWT");
    setDataMenu->addAction("seta");
}

















