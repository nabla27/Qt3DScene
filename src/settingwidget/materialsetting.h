#ifndef MATERIALSETTING_H
#define MATERIALSETTING_H

#include "abstractcomponentssettingwidget.h"


#include <Qt3DRender/QMaterial>
#include <QFormLayout>

class AbstractMaterialSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit AbstractMaterialSettingWidget(Qt3DRender::QMaterial *material, const QString& name,  QWidget *parent);

    virtual AbstractComponentsSettingWidget *const clone() const override = 0;

protected:
    QFormLayout *fLayout;

private:
    Qt3DRender::QMaterial *material;
};



#include <Qt3DExtras/QPhongMaterial>
class PhongMaterialSettingWidget : public AbstractMaterialSettingWidget
{
    Q_OBJECT
public:
    explicit PhongMaterialSettingWidget(Qt3DExtras::QPhongMaterial *material, QWidget *parent);

    AbstractComponentsSettingWidget *const clone() const override;

private:
    Qt3DExtras::QPhongMaterial *material;
};




#endif // MATERIALSETTING_H
