#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H
#include "abstractcomponentssettingwidget.h"
#include <Qt3DCore/QTransform>

class TransformWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit TransformWidget(Qt3DCore::QTransform *transform, QWidget *parent);

    AbstractComponentsSettingWidget *const clone() const override;

protected:
    void setupContentsLayout();

protected:
    Qt3DCore::QTransform *transform;
};

#endif // TRANSFORMWIDGET_H
