#ifndef ANIMATION_SETTING_H
#define ANIMATION_SETTING_H
#include "src/components-setting/abstractcomponentssettingwidget.h"
#include "animation.h"
#include <QSequentialAnimationGroup>
#include <Qt3DCore/QEntity>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QSlider>

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













class AbstractAnimationSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit AbstractAnimationSettingWidget(Qt3DCore::QComponent *target,
                                            AbstractAnimation *animation,
                                            const QString& name,
                                            QWidget *parent,
                                            const bool isSubComponent = false);

    virtual AbstractComponentsSettingWidget *const clone() const = 0;

protected:
    void addFormRow(const QString& text, QWidget *w) { animationFLayout->addRow(text, w); }
    void addSettingWidget(QWidget *w) { animationVLayout->addWidget(w); }

private slots:
    void setLoopCount(const int value);

private:
    AbstractAnimation *animation;
    QVBoxLayout *animationVLayout;
    QFormLayout *animationFLayout;
    AnimationControllBar *controllBar;
    QSpinBox *durationSpinBox;
    QSpinBox *loopCountSpinBox;
};














#endif // ANIMATION_H
