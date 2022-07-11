#include "layout.h"


#include <QHBoxLayout>
#include <QLabel>

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

void Form3DDoubleSpin::setMinimum(const double &min)
{
    xSpinBox->setMinimum(min);
    ySpinBox->setMinimum(min);
    zSpinBox->setMinimum(min);
}

void Form3DDoubleSpin::setMaximum(const double &max)
{
    xSpinBox->setMaximum(max);
    ySpinBox->setMaximum(max);
    zSpinBox->setMaximum(max);
}

void Form3DDoubleSpin::emitChangedValue()
{
    emit valueChanged(QVector3D(xSpinBox->value(), ySpinBox->value(), zSpinBox->value()));
}








#include <QColorDialog>

ColorDialogButton::ColorDialogButton(const QString& text, QWidget *parent)
    : QPushButton(text, parent)
    , colorDialog(new QColorDialog(parent))
{
    connect(this, &ColorDialogButton::released, colorDialog, &QColorDialog::show);
    connect(colorDialog, &QColorDialog::currentColorChanged, this, &ColorDialogButton::setButtonColor);
    connect(colorDialog, &QColorDialog::currentColorChanged, this, &ColorDialogButton::colorChanged);
}

void ColorDialogButton::setColor(const QColor& color)
{
    colorDialog->setCurrentColor(color);
}

void ColorDialogButton::setButtonColor(const QColor& color)
{
    const QString colorName = color.name();
    setStyleSheet(QString("QPushButton { background: %1 }").arg(colorName));
    setText(colorName);
    setToolTip(colorName);
}






#include <QLineEdit>
#include <QFormLayout>
#include <QToolButton>
#include <QFileDialog>
DllSelectorWidget::DllSelectorWidget(const QStringList& funcNameList,
                                     QWidget *parent)
    : QWidget(parent)
    , dllPathEdit(new QLineEdit(this))
    , symbolNameEdit(new QLineEdit(this))
    , requestLoadButton(new QPushButton("Load", this))
    , requestUnloadButton(new QPushButton("Unload", this))
    , validPalette(dllPathEdit->palette())
    , invalidPalette(dllPathEdit->palette())
{
    QFormLayout *fLayout = new QFormLayout(this);
    QHBoxLayout *dllPathEditLayout = new QHBoxLayout;
    QToolButton *openFileDialogButton = new QToolButton(this);
    QFileDialog *fileDialog = new QFileDialog(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    setLayout(fLayout);
    fLayout->addRow("DLL Path", dllPathEditLayout);
    dllPathEditLayout->addWidget(dllPathEdit);
    dllPathEditLayout->addWidget(openFileDialogButton);
    fLayout->addRow("Symbol Name", symbolNameEdit);

    for(const QString& funcName : funcNameList)
    {
        QLabel *funcNameLabel = new QLabel(funcName, this);
        funcNameLabel->setObjectName(funcName);
        funcNameLabels.append(funcNameLabel);
        fLayout->addRow("", funcNameLabel);
    }

    fLayout->addRow("", buttonLayout);
    buttonLayout->addWidget(requestLoadButton);
    buttonLayout->addWidget(requestUnloadButton);

    openFileDialogButton->setText("...");
    openFileDialogButton->setToolTip("open file dialog");
    fileDialog->setFileMode(QFileDialog::FileMode::ExistingFile);
    fileDialog->setNameFilter("*.dll");
    invalidPalette.setColor(QPalette::Text, Qt::red);
    requestUnloadButton->setEnabled(false);

    setContentsMargins(0, 0, 0, 0);
    fLayout->setContentsMargins(0, 0, 0, 0);
    dllPathEditLayout->setSpacing(0);
    dllPathEditLayout->setContentsMargins(0, 0, 0, 0);

    connect(dllPathEdit, &QLineEdit::textEdited, this, &DllSelectorWidget::dllPathEdited);
    connect(openFileDialogButton, &QToolButton::released, fileDialog, &QFileDialog::show);
    connect(fileDialog, &QFileDialog::fileSelected, this, &DllSelectorWidget::dllPathEdited);
    connect(fileDialog, &QFileDialog::fileSelected, dllPathEdit, &QLineEdit::setText);
    connect(symbolNameEdit, &QLineEdit::textEdited, this, &DllSelectorWidget::symbolNameEdited);
    connect(symbolNameEdit, &QLineEdit::textEdited, this, &DllSelectorWidget::changeFuncName);
    connect(requestLoadButton, &QPushButton::released, this, &DllSelectorWidget::loadRequested);
    connect(requestUnloadButton, &QPushButton::released, this, &DllSelectorWidget::unloadRequested);
}

void DllSelectorWidget::receiveDllState(const DllSelectorWidget::DllState& state)
{
    dllPathEdit->setPalette(validPalette);
    requestLoadButton->setEnabled(true);
    requestUnloadButton->setEnabled(false);

    switch(state)
    {
    case DllState::FailedToLoad:
    {
        dllPathEdit->setPalette(invalidPalette);
        return;
    }
    case DllState::FailedToResolve:
    {
        return;
    }
    case DllState::Resolved:
    {
        requestLoadButton->setEnabled(false);
        requestUnloadButton->setEnabled(true);
        return;
    }
    case DllState::Unloaded:
    {
        requestLoadButton->setEnabled(true);
        requestUnloadButton->setEnabled(false);
        return;
    }
    default:
        break;
    }
}

void DllSelectorWidget::changeFuncName(const QString& symbol)
{
    for(QLabel *const label : funcNameLabels)
        label->setText(symbol + label->objectName());
}











