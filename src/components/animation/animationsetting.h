#ifndef ANIMATION_SETTING_H
#define ANIMATION_SETTING_H
#include "src/components/abstractcomponentssettingwidget.h"
#include "animation.h"
#include <QSequentialAnimationGroup>
#include <Qt3DCore/QEntity>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QSlider>
#include <QListWidgetItem>

class QListWidget;


class AnimationEditor;
class AnimationGroupSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit AnimationGroupSettingWidget(Qt3DCore::QEntity *entity, Qt3DCore::QComponent *c, QWidget *parent);

    AbstractComponentsSettingWidget *const clone() const override { return nullptr; } //DEBUG

protected:
    Qt3DCore::QEntity *entity;
    QSequentialAnimationGroup *const animation;

private:
    QToolBar *controller;
    QAction *playAction;
    QAction *stopAction;
    QAction *openEditorAction;
    QSpinBox *timeSpinBox;
    AnimationEditor *animationEditor;
};

















class AnimationEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationEditor(QWidget *parent);
};










class AnimationControllBar : public QToolBar
{
    Q_OBJECT
public:
    explicit AnimationControllBar(QWidget *parent, AbstractAnimation *animation);

protected:
    QAction *playAction;
    QAction *stopAction;
    QSpinBox *timeSpinBox;
    QSlider *timeSlider;

private slots:
    void play();
    void reset();
    void setIconPlay();
    void setMaximumValue(const int duration);

private:
    void setConnectionState(const AbstractAnimation::State& newState, const AbstractAnimation::State& oldState);

private:
    AbstractAnimation *animation;

    QMetaObject::Connection spinBoxToTime;
    QMetaObject::Connection timeToSpinBox;
};












class SelectControllerWidget;
class AbstractAnimationSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit AbstractAnimationSettingWidget(Qt3DCore::QEntity *entity,
                                            AbstractAnimation *animation,
                                            QWidget *parent);

    AbstractComponentsSettingWidget *const clone() const override { return nullptr; }

private slots:
    void setLoopCount(const int value);
    void receiveCreatedController(AbstractController *controller);

private:
    AbstractAnimation *animation;
    AnimationControllBar *controllBar;
    SelectControllerWidget *selectControllerWidget;
    QWidget *controllerSettingWidget;

    QVBoxLayout *vLayout;
    QSpinBox *durationSpinBox;
    QSpinBox *loopCountSpinBox;
    QLineEdit *controllerNameEdit;
};








class SelectControllerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectControllerWidget(QWidget *parent, Qt3DCore::QEntity *entity);

    enum class ControllerTarget { Transform, GridMesh, };
    enum class ControllerType { TransformDLLController = 0,
                                GridMeshDLLController = 100, };
    static constexpr int enumStride = 100;
    Q_ENUM(ControllerTarget)
    Q_ENUM(ControllerType)

private slots:
    void setTargetComponensList(const QString& filter);
    void changeControllerList(const QModelIndex& index);
    void changeComponentsList(const QModelIndex& index);
    void setControllerFilter(const QString& filter);
    void createController();

private:
    template <typename T>
    void getObjectNamesFromComponents(const QList<T*>& componentsList, QStringList& list);

private:
    Qt3DCore::QEntity *entity;

    QListWidget *targetComponentsListWidget;
    QListWidget *controllerListWidget;
    QListWidget *componentsListWidget;

    QString controllerFilter;

    class ListItem : public QListWidgetItem
    {
    public:
        ListItem(const QString& name, QListWidget *w, const int enumIndex)
            : QListWidgetItem(name, w)
            , enumIndex(enumIndex) {}
    public:
        const int enumIndex;
    };

signals:
    void controllerCreated(AbstractController* controller);

};














#endif // ANIMATION_H
