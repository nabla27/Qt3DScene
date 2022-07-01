#include "transformwidget.h"



#include <QFormLayout>
#include "layout.h"
TransformWidget::TransformWidget(Qt3DCore::QTransform *transform, QWidget *parent)
    : AbstractComponentsSettingWidget(transform, "Transform", parent)
    , transform(transform)
{
    setupContentsLayout();
}

void TransformWidget::setupContentsLayout()
{
    QFormLayout *fLayout = new QFormLayout(contents);
    Form3DDoubleSpin *positionSpinBox = new Form3DDoubleSpin(contents);
    Form3DDoubleSpin *rotationSpinBox = new Form3DDoubleSpin(contents);
    Form3DDoubleSpin *scale3dSpinBox = new Form3DDoubleSpin(contents);
    QDoubleSpinBox *scaleSpinBox = new QDoubleSpinBox(contents);

    contents->setLayout(fLayout);
    fLayout->addRow("Position", positionSpinBox);
    fLayout->addRow("Rotation", rotationSpinBox);
    fLayout->addRow("Scale3D", scale3dSpinBox);
    fLayout->addRow("Scale", scaleSpinBox);

    connect(transform, &Qt3DCore::QTransform::translationChanged, positionSpinBox, &Form3DDoubleSpin::setValue);
    connect(transform, &Qt3DCore::QTransform::rotationXChanged, rotationSpinBox, &Form3DDoubleSpin::setXValue);
    connect(transform, &Qt3DCore::QTransform::rotationYChanged, rotationSpinBox, &Form3DDoubleSpin::setYValue);
    connect(transform, &Qt3DCore::QTransform::rotationZChanged, rotationSpinBox, &Form3DDoubleSpin::setZValue);
    connect(transform, &Qt3DCore::QTransform::scale3DChanged, scale3dSpinBox, &Form3DDoubleSpin::setValue);
    connect(transform, &Qt3DCore::QTransform::scaleChanged, scaleSpinBox, &QDoubleSpinBox::setValue);

    connect(positionSpinBox, &Form3DDoubleSpin::valueChanged, transform, &Qt3DCore::QTransform::setTranslation);
    connect(rotationSpinBox, &Form3DDoubleSpin::xValueChanged, transform, &Qt3DCore::QTransform::setRotationX);
    connect(rotationSpinBox, &Form3DDoubleSpin::yValueChanged, transform, &Qt3DCore::QTransform::setRotationY);
    connect(rotationSpinBox, &Form3DDoubleSpin::zValueChanged, transform, &Qt3DCore::QTransform::setRotationZ);
    connect(scale3dSpinBox, &Form3DDoubleSpin::valueChanged, transform, &Qt3DCore::QTransform::setScale3D);
    connect(scaleSpinBox, &QDoubleSpinBox::valueChanged, transform, &Qt3DCore::QTransform::setScale);
}

AbstractComponentsSettingWidget *const TransformWidget::clone() const
{
    return new TransformWidget(transform, parentWidget());
}
