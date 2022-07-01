#include "layout.h"


#include <QHBoxLayout>
Form3DDoubleSpin::Form3DDoubleSpin(QWidget *parent)
    : QWidget(parent)
    , xLabel(new QLabel(" X", this))
    , yLabel(new QLabel(" Y", this))
    , zLabel(new QLabel(" Z", this))
    , xSpinBox(new QDoubleSpinBox(this))
    , ySpinBox(new QDoubleSpinBox(this))
    , zSpinBox(new QDoubleSpinBox(this))
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    setLayout(hLayout);
    hLayout->addWidget(xLabel);
    hLayout->addWidget(xSpinBox);
    hLayout->addWidget(yLabel);
    hLayout->addWidget(ySpinBox);
    hLayout->addWidget(zLabel);
    hLayout->addWidget(zSpinBox);

    xLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    yLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    zLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    xSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ySpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    zSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    hLayout->setSpacing(2);
    hLayout->setContentsMargins(0, 0, 0, 0);

    connect(xSpinBox, &QDoubleSpinBox::valueChanged, this, &Form3DDoubleSpin::xValueChanged);
    connect(ySpinBox, &QDoubleSpinBox::valueChanged, this, &Form3DDoubleSpin::yValueChanged);
    connect(zSpinBox, &QDoubleSpinBox::valueChanged, this, &Form3DDoubleSpin::zValueChanged);
    connect(xSpinBox, &QDoubleSpinBox::valueChanged, this, &Form3DDoubleSpin::emitChangedValue);
    connect(ySpinBox, &QDoubleSpinBox::valueChanged, this, &Form3DDoubleSpin::emitChangedValue);
    connect(zSpinBox, &QDoubleSpinBox::valueChanged, this, &Form3DDoubleSpin::emitChangedValue);
}

void Form3DDoubleSpin::emitChangedValue()
{
    emit valueChanged(QVector3D(xSpinBox->value(), ySpinBox->value(), zSpinBox->value()));
}
