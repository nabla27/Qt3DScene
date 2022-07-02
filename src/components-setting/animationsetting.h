#ifndef ANIMATION_H
#define ANIMATION_H
#include "abstractcomponentssettingwidget.h"
#include <QSequentialAnimationGroup>
#include <Qt3DCore/QEntity>
#include <QVBoxLayout>
#include <QSpinBox>

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
};


#endif // ANIMATION_H
