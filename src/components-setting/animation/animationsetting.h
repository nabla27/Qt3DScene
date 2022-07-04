#ifndef ANIMATION_SETTING_H
#define ANIMATION_SETTING_H
#include "src/components-setting/abstractcomponentssettingwidget.h"
#include "animation.h"
#include <QSequentialAnimationGroup>
#include <Qt3DCore/QEntity>
#include <QVBoxLayout>
#include <QSpinBox>

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

private slots:
    void play();
    void reset();
    void setIconPlay();

private:
    AbstractAnimation *animation;
};














#endif // ANIMATION_H
