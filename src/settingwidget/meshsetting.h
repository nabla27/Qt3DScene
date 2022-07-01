#ifndef MESHSETTING_H
#define MESHSETTING_H

#include "abstractcomponentssettingwidget.h"


#include <Qt3DExtras/QConeMesh>
class ConeMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit ConeMeshSettingWidget(Qt3DExtras::QConeMesh *mesh, QWidget *parent);

    AbstractComponentsSettingWidget *const clone() const override;

protected:
    Qt3DExtras::QConeMesh *mesh;
};

#endif // MESHSETTING_H
