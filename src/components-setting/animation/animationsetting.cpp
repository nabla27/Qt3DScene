#include "animationsetting.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QApplication>

//DEBUG
#include "transformanimation.h"
#include <Qt3DCore/QTransform>

AnimationGroupSettingWidget::AnimationGroupSettingWidget(Qt3DCore::QEntity *entity, Qt3DCore::QComponent *c, QWidget *parent)
    : AbstractComponentsSettingWidget(c, "AnimationGroup", parent)
    , entity(entity)
    , animation(new QSequentialAnimationGroup(c))
    , controller(new QToolBar(contents))
    , playAction(new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaPlay), "Play", controller))
    , stopAction(new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaStop), "Stop", controller))
    , openEditorAction(new QAction(IconSet::gear, "Open editor", controller))
    , timeSpinBox(new QSpinBox(controller))
    , animationEditor(new AnimationEditor(nullptr))
{
    setIcon(IconSet::animationgroup);

    QVBoxLayout *vLayout = new QVBoxLayout(contents);

    contents->setLayout(vLayout);
    vLayout->addWidget(controller);
    controller->addAction(playAction);
    controller->addAction(stopAction);
    controller->addWidget(timeSpinBox);
    controller->addAction(openEditorAction);

    controller->setIconSize(QSize(iconSize, iconSize));
    timeSpinBox->setMaximum(10000000);

    connect(openEditorAction, &QAction::triggered, animationEditor, &AnimationEditor::show);
    connect(this, &AnimationGroupSettingWidget::destroyed, animationEditor, &AnimationEditor::deleteLater);




    AnimationControllBar *bar = new AnimationControllBar(this, new TransformAnimation(qobject_cast<Qt3DCore::QTransform*>(c)));
    vLayout->addWidget(bar);
}





























#include "src/utility.h"
#include <QScreen>

AnimationEditor::AnimationEditor(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(getRectFromScreenRatio(screen()->size(), 0.5f, 0.5f));
    setWindowTitle("Qt3D Scene  Animation Editor");
}










AnimationControllBar::AnimationControllBar(QWidget *parent, AbstractAnimation *animation)
    : QToolBar(parent)
    , playAction(new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaPlay), "Play", this))
    , stopAction(new QAction(QApplication::style()->standardIcon(QStyle::SP_MediaStop), "Stop", this))
    , timeSpinBox(new QSpinBox(this))
    , animation(animation)
{
    addAction(playAction);
    addAction(stopAction);
    addWidget(timeSpinBox);

    connect(playAction, &QAction::triggered, this, &AnimationControllBar::play);
    connect(stopAction, &QAction::triggered, this, &AnimationControllBar::reset);
    connect(timeSpinBox, &QSpinBox::valueChanged, animation, &AbstractAnimation::setCurrentTime);
    connect(animation, &AbstractAnimation::currentTimeChanged, timeSpinBox, &QSpinBox::setValue);
    connect(animation, &AbstractAnimation::finished, this, &AnimationControllBar::setIconPlay);
    connect(animation, &AbstractAnimation::durationChanged, timeSpinBox, &QSpinBox::setMaximum);

    setIconSize(QSize(15, 15));

    animation->setDuration(1000);
}

void AnimationControllBar::play()
{
    switch(animation->state())
    {
    case QAbstractAnimation::State::Running:
    {
        playAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        animation->pause();
        break;
    }
    case QAbstractAnimation::State::Paused:
    {
        playAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
        animation->resume();
        break;
    }
    case QAbstractAnimation::State::Stopped:
    {
        animation->setCurrentTime(0);
        animation->start();
        playAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
        break;
    }
    default:
        return;
    }
}

void AnimationControllBar::reset()
{
    animation->setCurrentTime(0);
}

void AnimationControllBar::setIconPlay()
{
    playAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
}














