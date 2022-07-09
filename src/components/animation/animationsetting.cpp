#include "animationsetting.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QApplication>

//DEBUG
#include "transformanimation.h"
#include "gridmeshanimation.h"
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




    //AnimationControllBar *bar = new AnimationControllBar(this, new AbstractAnimation(qobject_cast<Qt3DCore::QTransform*>(c)));
    //vLayout->addWidget(bar);
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
    , controllerSettingWidget(nullptr)
    , vLayout(new QVBoxLayout(contents))
    , durationSpinBox(new QSpinBox(contents))
    , loopCountSpinBox(new QSpinBox(contents))
    , controllerNameEdit(new QLineEdit("None"))
{
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
    selectControllerButton->setToolTip("select controller");

    selectControllerWidget->setContentsMargins(0, 0, 0, 0);
    selectControllerLayout->setContentsMargins(0, 0, 0, 0);
    selectControllerLayout->setSpacing(0);

    connect(durationSpinBox, &QSpinBox::valueChanged, animation, &AbstractAnimation::setDuration);
    connect(animation, &AbstractAnimation::durationChanged, durationSpinBox, &QSpinBox::setValue);
    connect(loopCountSpinBox, &QSpinBox::valueChanged, this, &AbstractAnimationSettingWidget::setLoopCount);
    connect(selectControllerButton, &QToolButton::released, selectControllerWidget, &SelectControllerWidget::show);
    connect(selectControllerWidget, &SelectControllerWidget::controllerCreated, this, &AbstractAnimationSettingWidget::receiveCreatedController);
}

void AbstractAnimationSettingWidget::setLoopCount(const int value)
{
    if(value == 0)
        animation->setLoopCount(-1);
    else
        animation->setLoopCount(value);
}

void AbstractAnimationSettingWidget::receiveCreatedController(AbstractController *controller)
{
    animation->setController(controller);
    if(controllerSettingWidget) delete controllerSettingWidget;
    controllerSettingWidget = controller->paramWidgets(contents);
    vLayout->addWidget(controllerSettingWidget);
    controllerNameEdit->setText(controller->objectName());
}










#include <QSplitter>
#include <QMessageBox>
#include <QInputDialog>

SelectControllerWidget::SelectControllerWidget(QWidget *parent, Qt3DCore::QEntity *entity)
    : QWidget(parent)
    , entity(entity)
    , targetComponentsListWidget(new QListWidget(this))
    , controllerListWidget(new QListWidget(this))
    , componentsListWidget(new QListWidget(this))
{
    setWindowTitle("Slect Animation Controller");
    setWindowFlag(Qt::WindowType::Window, true);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *targetComponentsListLayout = new QVBoxLayout;
    QVBoxLayout *controllerListLayout = new QVBoxLayout;
    QVBoxLayout *componentsListLayout = new QVBoxLayout;
    QHBoxLayout *targetComponentsListPanelLayout = new QHBoxLayout;
    QHBoxLayout *controllerListPanelLayout = new QHBoxLayout;
    QHBoxLayout *componentsListPanelLayout = new QHBoxLayout;
    QWidget *targetComponentsListArea = new QWidget(this);
    QWidget *controllerListArea = new QWidget(this);
    QWidget *componentsListArea = new QWidget(this);
    QLineEdit *targetComponentsFilterEdit = new QLineEdit(targetComponentsListWidget);
    QLineEdit *controllerFilterEdit = new QLineEdit(controllerListWidget);
    QSplitter *splitter = new QSplitter(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QSpacerItem *buttonSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);
    QPushButton *selectControllerButton = new QPushButton("Select", this);

    setLayout(vLayout);
    vLayout->addLayout(hLayout);
    hLayout->addWidget(splitter);
    splitter->addWidget(targetComponentsListArea);
    splitter->addWidget(controllerListArea);
    splitter->addWidget(componentsListArea);
    vLayout->addLayout(buttonLayout);
    buttonLayout->addItem(buttonSpacer);
    buttonLayout->addWidget(selectControllerButton);

    targetComponentsListArea->setLayout(targetComponentsListLayout);
    controllerListArea->setLayout(controllerListLayout);
    componentsListArea->setLayout(componentsListLayout);

    targetComponentsListLayout->addLayout(targetComponentsListPanelLayout);
    targetComponentsListLayout->addWidget(targetComponentsListWidget);
    targetComponentsListPanelLayout->addWidget(new QLabel("Target "));
    targetComponentsListPanelLayout->addWidget(targetComponentsFilterEdit);

    controllerListLayout->addLayout(controllerListPanelLayout);
    controllerListLayout->addWidget(controllerListWidget);
    controllerListPanelLayout->addWidget(new QLabel("Controller "));
    controllerListPanelLayout->addWidget(controllerFilterEdit);

    componentsListLayout->addLayout(componentsListPanelLayout);
    componentsListLayout->addWidget(componentsListWidget);
    componentsListPanelLayout->addWidget(new QLabel("Components "));

    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0, 0, 0, 0);

    targetComponentsListArea->setContentsMargins(0, 0, 0, 0);
    controllerListArea->setContentsMargins(0, 0, 0, 0);
    componentsListArea->setContentsMargins(0, 0, 0, 0);

    targetComponentsListLayout->setSpacing(0);
    controllerListLayout->setSpacing(0);
    componentsListLayout->setSpacing(0);
    targetComponentsListLayout->setContentsMargins(0, 0, 0, 0);
    controllerListLayout->setContentsMargins(0, 0, 0, 0);
    componentsListLayout->setContentsMargins(0, 0, 0, 0);

    connect(targetComponentsListWidget, &QListView::clicked, this, &SelectControllerWidget::changeControllerList);
    connect(targetComponentsListWidget, &QListView::clicked, this, &SelectControllerWidget::changeComponentsList);
    connect(targetComponentsFilterEdit, &QLineEdit::textEdited, this, &SelectControllerWidget::setTargetComponensList);
    connect(controllerFilterEdit, &QLineEdit::textEdited, this, &SelectControllerWidget::setControllerFilter);
    connect(selectControllerButton, &QPushButton::released, this, &SelectControllerWidget::createController);

    setTargetComponensList("");
}

void SelectControllerWidget::setTargetComponensList(const QString& filter)
{
    targetComponentsListWidget->clear();

    for(int i = 0;; ++i)
    {
        const QString itemName = enumToString(ControllerTarget(i));

        if(itemName.isEmpty()) break;

        if(itemName.contains(filter, Qt::CaseInsensitive))
        {
            targetComponentsListWidget->addItem(new ListItem(itemName, targetComponentsListWidget, i));
        }
    }
}

void SelectControllerWidget::changeControllerList(const QModelIndex& index)
{
    const int selectedIndex = static_cast<ListItem*>(targetComponentsListWidget->item(index.row()))->enumIndex;

    controllerListWidget->clear();

    for(int i = selectedIndex * enumStride;; ++i)
    {
        const QString itemName = enumToString(ControllerType(i));

        if(itemName.isEmpty()) break;

        if(itemName.contains(controllerFilter, Qt::CaseInsensitive))
        {
            controllerListWidget->addItem(new ListItem(itemName, controllerListWidget, i));
        }
    }
}

void SelectControllerWidget::changeComponentsList(const QModelIndex& index)
{
    const int selectedRow = index.row();
    QStringList list;

    switch(ControllerTarget(selectedRow))
    {
    case ControllerTarget::Transform:
        getObjectNamesFromComponents(entity->componentsOfType<Qt3DCore::QTransform>(), list);
        break;
    case ControllerTarget::GridMesh:
        getObjectNamesFromComponents(entity->componentsOfType<GridMesh>(), list);
        break;
    default:
        break;
    }

    componentsListWidget->clear();
    componentsListWidget->addItems(list);
}

void SelectControllerWidget::setControllerFilter(const QString& filter)
{
    controllerFilter = filter;
    changeControllerList(targetComponentsListWidget->currentIndex());
}

void SelectControllerWidget::createController()
{
    if(controllerListWidget->currentRow() == -1)
    {
        QMessageBox::warning(this, "Select Controller", "Controller is not selected.");
        return;
    }
    else if(componentsListWidget->currentRow() == -1)
    {
        QMessageBox::warning(this, "Select Controller", "Component is not selected");
        return;
    }

    AbstractController *controller = nullptr;

    const ControllerType controllerType = ControllerType(static_cast<ListItem*>(controllerListWidget->currentItem())->enumIndex);

    switch(controllerType)
    {
    case ControllerType::TransformDLLController:
    {
        controller = new TransformDllController(entity->componentsOfType<Qt3DCore::QTransform>().at(componentsListWidget->currentRow()));
        break;
    }
    case ControllerType::GridMeshDLLController:
    {
        controller = new GridMeshDllController(entity->componentsOfType<GridMesh>().at(componentsListWidget->currentRow()));
        break;
    }
    default:
        return;
    }

    controller->setObjectName(enumToString(controllerType));
    emit controllerCreated(controller);
    hide();
}

template <typename T>
void SelectControllerWidget::getObjectNamesFromComponents(const QList<T*>& componentsList, QStringList& list)
{
    for(const T * c : componentsList)
        list << static_cast<const Qt3DCore::QComponent*>(c)->objectName();
}













