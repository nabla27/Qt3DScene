#include "materialsetting.h"

AbstractMaterialSettingWidget::AbstractMaterialSettingWidget(Qt3DRender::QMaterial *material, const QString& name, QWidget *parent)
    : AbstractComponentsSettingWidget(material, name, parent)
    , fLayout(new QFormLayout(contents))
    , material(material)
{
    contents->setLayout(fLayout);
}




#include "layout.h"

PhongMaterialSettingWidget::PhongMaterialSettingWidget(Qt3DExtras::QPhongMaterial *material, QWidget *parent)
    : AbstractMaterialSettingWidget(material, "PhongMaterial", parent)
    , material(material)
{
    ColorDialogButton *ambientColorButton = new ColorDialogButton("", contents);
    ColorDialogButton *diffuseColorButton = new ColorDialogButton("", contents);
    ColorDialogButton *specularColorButton = new ColorDialogButton("", contents);
    QDoubleSpinBox *shininessSpinBox = new QDoubleSpinBox(contents);

    fLayout->addRow("Ambient", ambientColorButton);
    fLayout->addRow("Diffuse", diffuseColorButton);
    fLayout->addRow("Specular", specularColorButton);
    fLayout->addRow("Shininess", shininessSpinBox);

    shininessSpinBox->setMinimum(0);
    shininessSpinBox->setMaximum(100000);

    connect(ambientColorButton, &ColorDialogButton::colorChanged, material, &Qt3DExtras::QPhongMaterial::setAmbient);
    connect(diffuseColorButton, &ColorDialogButton::colorChanged, material, &Qt3DExtras::QPhongMaterial::setDiffuse);
    connect(specularColorButton, &ColorDialogButton::colorChanged, material, &Qt3DExtras::QPhongMaterial::setSpecular);
    connect(shininessSpinBox, &QDoubleSpinBox::valueChanged, material, &Qt3DExtras::QPhongMaterial::setShininess);

    connect(material, &Qt3DExtras::QPhongMaterial::ambientChanged, ambientColorButton, &ColorDialogButton::setColor);
    connect(material, &Qt3DExtras::QPhongMaterial::diffuseChanged, diffuseColorButton, &ColorDialogButton::setColor);
    connect(material, &Qt3DExtras::QPhongMaterial::specularChanged, specularColorButton, &ColorDialogButton::setColor);
    connect(material, &Qt3DExtras::QPhongMaterial::shininessChanged, shininessSpinBox, &QDoubleSpinBox::setValue);

    ambientColorButton->setColor(material->ambient());
    diffuseColorButton->setColor(material->diffuse());
    specularColorButton->setColor(material->specular());
    shininessSpinBox->setValue(material->shininess());
}

AbstractComponentsSettingWidget *const PhongMaterialSettingWidget::clone() const
{
    return new PhongMaterialSettingWidget(material, parentWidget());
}
