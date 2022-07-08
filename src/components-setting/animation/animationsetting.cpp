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




    AnimationControllBar *bar = new AnimationControllBar(this, new AbstractAnimation(qobject_cast<Qt3DCore::QTransform*>(c)));
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
    , timeSlider(new QSlider(Qt::Orientation::Horizontal, this))
    , animation(animation)
{
    addAction(playAction);
    addAction(stopAction);
    addWidget(new QLabel("Time:"));
    addWidget(timeSpinBox);
    addWidget(timeSlider);

    connect(playAction, &QAction::triggered, this, &AnimationControllBar::play);
    connect(stopAction, &QAction::triggered, this, &AnimationControllBar::reset);
    connect(animation, &AbstractAnimation::finished, this, &AnimationControllBar::setIconPlay);
    connect(animation, &AbstractAnimation::durationChanged, this, &AnimationControllBar::setMaximumValue);
    connect(animation, &AbstractAnimation::stateChanged, this, &AnimationControllBar::setConnectionState);
    connect(timeSpinBox, &QSpinBox::valueChanged, timeSlider, &QSlider::setValue);
    connect(timeSlider, &QSlider::valueChanged, timeSpinBox, &QSpinBox::setValue);
    spinBoxToTime = connect(timeSpinBox, &QSpinBox::valueChanged, animation, &AbstractAnimation::setCurrentTime);

    setIconSize(QSize(15, 15));

    animation->setDuration(1000);
    setContentsMargins(0, 0, 0, 0);
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
        playAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
        animation->setCurrentTime(0);
        animation->start();
        break;
    }
    default:
        return;
    }
}

void AnimationControllBar::reset()
{
    animation->setCurrentTime(0);
    timeSpinBox->setValue(0);        //State::PausedまたはState::Stoppedの場合に必要
}

void AnimationControllBar::setIconPlay()
{
    playAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
}

void AnimationControllBar::setMaximumValue(const int duration)
{
    timeSpinBox->setMaximum(duration);
    timeSlider->setMaximum(duration);
}

/* timeSpinBox::valueChanged() と animation::currentTimeChanged() のどちらか一方向のみスロットをつなげる
 * timeSpinBox::valueChanged() -> animation::setCurrentTime() -> animation::currentTimeChanged() -> timeSpinBox::setValue()
 * と両方向のスロットをつなげると, setCurrentTime()とcurrentTimeChanged()の間に処理時間があると，timeSpinBox::valueChanged()とtimeSpinBox::setValue()間で
 * 無限ループに陥る可能性が出てくるため。
 * アニメーションが止まっているとき(Paused,Stopped)には timeSpinBox::valueChanged() -> animation::setCurrentTime()
 * アニメーションが開始されているとき(Running)には animation::currentTimeChanged() -> timeSpinBox::setValue()
 */
void AnimationControllBar::setConnectionState(const AbstractAnimation::State &newState, const AbstractAnimation::State&)
{
    switch(newState)
    {
    case AbstractAnimation::State::Paused:
    case AbstractAnimation::State::Stopped:
    {
        animation->disconnect(timeToSpinBox);
        spinBoxToTime = connect(timeSpinBox, &QSpinBox::valueChanged, animation, &AbstractAnimation::setCurrentTime);
        break;
    }
    case AbstractAnimation::State::Running:
    {
        timeSpinBox->disconnect(spinBoxToTime);
        timeToSpinBox = connect(animation, &AbstractAnimation::currentTimeChanged, timeSpinBox, &QSpinBox::setValue);
        break;
    }
    default:
        break;
    }
}









#include <QLineEdit>
#include <QToolButton>
AbstractAnimationSettingWidget::AbstractAnimationSettingWidget(Qt3DCore::QEntity *entity,
                                                               AbstractAnimation *animation,
                                                               QWidget *parent)
    : AbstractComponentsSettingWidget(nullptr, "Animation", parent)
    , animation(animation)
    , controllBar(new AnimationControllBar(contents, animation))
    , selectControllerWidget(new SelectControllerWidget(contents, entity))
    , durationSpinBox(new QSpinBox(contents))
    , loopCountSpinBox(new QSpinBox(contents))
    , controllerNameEdit(new QLineEdit("None"))
{
    QVBoxLayout *vLayout = new QVBoxLayout(contents);
    QFormLayout *fLayout = new QFormLayout;
    QWidget *selectControllerForm = new QWidget(contents);
    QHBoxLayout *selectControllerLayout = new QHBoxLayout(selectControllerForm);
    QToolButton *selectControllerButton = new QToolButton(selectControllerForm);

    contents->setLayout(vLayout);
    vLayout->addWidget(controllBar);
    vLayout->addLayout(fLayout);
    fLayout->addRow("Duration", durationSpinBox);
    fLayout->addRow("Loop Count", loopCountSpinBox);
    fLayout->addRow("Controller", selectControllerForm);
    selectControllerForm->setLayout(selectControllerLayout);
    selectControllerLayout->addWidget(controllerNameEdit);
    selectControllerLayout->addWidget(selectControllerButton);

    durationSpinBox->setMaximum(10000000);
    loopCountSpinBox->setMaximum(1000000);
    durationSpinBox->setValue(animation->duration());
    loopCountSpinBox->setValue((animation->loopCount() == -1) ? 0 : animation->loopCount());
    controllerNameEdit->setReadOnly(true);
    selectControllerButton->setText("...");

    selectControllerWidget->setContentsMargins(0, 0, 0, 0);
    selectControllerLayout->setContentsMargins(0, 0, 0, 0);
    selectControllerLayout->setSpacing(0);

    //DEBUG
    TransformDllController *c = new TransformDllController(entity->componentsOfType<Qt3DCore::QTransform>().at(0));
    animation->setController(c);
    vLayout->addWidget(c->paramWidgets(contents));

    connect(durationSpinBox, &QSpinBox::valueChanged, animation, &AbstractAnimation::setDuration);
    connect(animation, &AbstractAnimation::durationChanged, durationSpinBox, &QSpinBox::setValue);
    connect(loopCountSpinBox, &QSpinBox::valueChanged, this, &AbstractAnimationSettingWidget::setLoopCount);
    connect(selectControllerButton, &QToolButton::released, selectControllerWidget, &SelectControllerWidget::show);
}

void AbstractAnimationSettingWidget::setLoopCount(const int value)
{
    if(value == 0)
        animation->setLoopCount(-1);
    else
        animation->setLoopCount(value);
}











SelectControllerWidget::SelectControllerWidget(QWidget *parent, Qt3DCore::QEntity *entity)
    : QWidget(parent)
    , entity(entity)
{
    setWindowTitle("Slect Animation Controller");
    setWindowFlag(Qt::WindowType::Window, true);


    QVBoxLayout *vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    vLayout->addWidget(new QLineEdit("test", this));
}















