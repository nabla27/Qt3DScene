#include "animationsetting.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QApplication>
AnimationGroupSettingWidget::AnimationGroupSettingWidget(Qt3DCore::QEntity *entity, Qt3DCore::QComponent *c, QWidget *parent)
    : AbstractComponentsSettingWidget(c, "AnimationGroup", parent)
    , entity(entity)
    , animation(new QSequentialAnimationGroup(c))
    , controller(new QToolBar(contents))
    , playAction(new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaPlay), "Play", controller))
    , stopAction(new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaStop), "Stop", controller))
    , openEditorAction(new QAction(QIcon(":/icon/gear"), "Open editor", controller))
    , timeSpinBox(new QSpinBox(controller))
{
    setIcon(":/icon/animation-group");

    QVBoxLayout *vLayout = new QVBoxLayout(contents);

    contents->setLayout(vLayout);
    vLayout->addWidget(controller);
    controller->addAction(playAction);
    controller->addAction(stopAction);
    controller->addWidget(timeSpinBox);
    controller->addAction(openEditorAction);

    controller->setIconSize(QSize(iconSize, iconSize));
    timeSpinBox->setMaximum(10000000);
}
