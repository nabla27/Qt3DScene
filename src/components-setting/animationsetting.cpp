#include "animationsetting.h"

AnimationGroupSettingWidget::AnimationGroupSettingWidget(Qt3DCore::QEntity *entity, Qt3DCore::QComponent *c, QWidget *parent)
    : AbstractComponentsSettingWidget(c, "AnimationGroup", parent)
    , entity(entity)
    , animation(new QSequentialAnimationGroup(c))
{
    setIcon(":/icon/animation-group");
}
