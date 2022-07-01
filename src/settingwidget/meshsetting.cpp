#include "meshsetting.h"

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QFormLayout>

ConeMeshSettingWidget::ConeMeshSettingWidget(Qt3DExtras::QConeMesh *mesh, QWidget *parent)
    : AbstractComponentsSettingWidget(mesh, "ConeMesh", parent)
    , mesh(mesh)
{
    QFormLayout *fLayout = new QFormLayout(contents);
    QDoubleSpinBox *topRadiusSpinBox = new QDoubleSpinBox(contents);
    QDoubleSpinBox *bottomRadiusSpinBox = new QDoubleSpinBox(contents);
    QDoubleSpinBox *lengthSpinBox = new QDoubleSpinBox(contents);
    QSpinBox *ringsSpinBox = new QSpinBox(contents);
    QSpinBox *slicesSpinBox = new QSpinBox(contents);
    QCheckBox *hasTopEndcapCheck = new QCheckBox(contents);
    QCheckBox *hasBottomEndcapCheck = new QCheckBox(contents);

    fLayout->addRow("Top Radius", topRadiusSpinBox);
    fLayout->addRow("Bottom Radius", bottomRadiusSpinBox);
    fLayout->addRow("Length", lengthSpinBox);
    fLayout->addRow("Rings", ringsSpinBox);
    fLayout->addRow("Slices", slicesSpinBox);
    fLayout->addRow("Top Endcap", hasTopEndcapCheck);
    fLayout->addRow("Bottom Endcap", hasBottomEndcapCheck);

    constexpr int maxValue = 100000;
    topRadiusSpinBox->setMaximum(maxValue);
    bottomRadiusSpinBox->setMaximum(maxValue);
    lengthSpinBox->setMinimum(0);
    lengthSpinBox->setMaximum(maxValue);
    ringsSpinBox->setMinimum(1);
    ringsSpinBox->setMaximum(maxValue);
    slicesSpinBox->setMinimum(1);
    slicesSpinBox->setMaximum(maxValue);

    connect(topRadiusSpinBox, &QDoubleSpinBox::valueChanged, mesh, &Qt3DExtras::QConeMesh::setTopRadius);
    connect(bottomRadiusSpinBox, &QDoubleSpinBox::valueChanged, mesh, &Qt3DExtras::QConeMesh::setBottomRadius);
    connect(lengthSpinBox, &QDoubleSpinBox::valueChanged, mesh, &Qt3DExtras::QConeMesh::setLength);
    connect(ringsSpinBox, &QSpinBox::valueChanged, mesh, &Qt3DExtras::QConeMesh::setRings);
    connect(slicesSpinBox, &QSpinBox::valueChanged, mesh, &Qt3DExtras::QConeMesh::setSlices);
    connect(hasTopEndcapCheck, &QCheckBox::clicked, mesh, &Qt3DExtras::QConeMesh::setHasTopEndcap);
    connect(hasBottomEndcapCheck, &QCheckBox::clicked, mesh, &Qt3DExtras::QConeMesh::setHasBottomEndcap);

    connect(mesh, &Qt3DExtras::QConeMesh::topRadiusChanged, topRadiusSpinBox, &QDoubleSpinBox::setValue);
    connect(mesh, &Qt3DExtras::QConeMesh::bottomRadiusChanged, bottomRadiusSpinBox, &QDoubleSpinBox::setValue);
    connect(mesh, &Qt3DExtras::QConeMesh::lengthChanged, lengthSpinBox, &QDoubleSpinBox::setValue);
    connect(mesh, &Qt3DExtras::QConeMesh::ringsChanged, ringsSpinBox, &QSpinBox::setValue);
    connect(mesh, &Qt3DExtras::QConeMesh::slicesChanged, slicesSpinBox, &QSpinBox::setValue);
    connect(mesh, &Qt3DExtras::QConeMesh::hasTopEndcapChanged, hasTopEndcapCheck, &QCheckBox::setChecked);
    connect(mesh, &Qt3DExtras::QConeMesh::hasBottomEndcapChanged, hasBottomEndcapCheck, &QCheckBox::setChecked);

    topRadiusSpinBox->setValue(mesh->topRadius());
    bottomRadiusSpinBox->setValue(mesh->bottomRadius());
    lengthSpinBox->setValue(mesh->length());
    ringsSpinBox->setValue(mesh->rings());
    slicesSpinBox->setValue(mesh->slices());
    hasTopEndcapCheck->setChecked(mesh->hasTopEndcap());
    hasBottomEndcapCheck->setChecked(mesh->hasBottomEndcap());
}

AbstractComponentsSettingWidget *const ConeMeshSettingWidget::clone() const
{
    return new ConeMeshSettingWidget(mesh, parentWidget());
}
