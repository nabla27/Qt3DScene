#include "transformanimation.h"
#include <QVector3D>




#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
#include <QPushButton>
#include <QCheckBox>
TransformDllController::TransformDllController(Qt3DCore::QTransform *transform, QObject *parent)
    : AbstractController(parent)
    , transform(transform)
    , updateTransformFunc(nullptr)
    , funcName(QString())
    , dllPath(QString())
{
    connect(transform, &Qt3DCore::QTransform::destroyed, this, &TransformDllController::deleteLater);
}

QWidget* TransformDllController::paramWidgets(QWidget *parent) const
{
    TransformDllControllerSettingWidget *widget = new TransformDllControllerSettingWidget(parent);

    connect(widget, &TransformDllControllerSettingWidget::dllPathEdited, this, &TransformDllController::setDllPath);
    connect(widget, &TransformDllControllerSettingWidget::funcNameEdited, this, &TransformDllController::setFuncName);
    connect(widget, &TransformDllControllerSettingWidget::dllLoadRequested, this, &TransformDllController::loadDll);
    connect(widget, &TransformDllControllerSettingWidget::dllUnloadRequested, this, &TransformDllController::unloadDll);
    connect(this, &TransformDllController::stateChanged, widget, &TransformDllControllerSettingWidget::receiveDllState);

    return widget;
}

void TransformDllController::update(const int& msec)
{
    if(!updateTransformFunc) return;

    mutex.lock();

    float p[10] = { transform->translation().x(), transform->translation().y(), transform->translation().z(),
                    transform->rotationX(), transform->rotationY(), transform->rotationZ(),
                    transform->scale3D().x(), transform->scale3D().y(), transform->scale3D().z(),
                    (float)msec / 1000 };

    updateTransformFunc(p);

    transform->setTranslation(QVector3D(p[0], p[1], p[2]));
    transform->setRotationX(p[3]);
    transform->setRotationY(p[4]);
    transform->setRotationZ(p[5]);
    transform->setScale3D(QVector3D(p[6], p[7], p[8]));

    mutex.unlock();

    emit updated(msec);
}

void TransformDllController::setFuncName(const QString& funcName)
{
    this->funcName = funcName;
}

void TransformDllController::setDllPath(const QString& path)
{
    dllPath = path;
}

void TransformDllController::loadDll()
{
    lib.setFileName(dllPath);

    if(lib.load())
    {
        emit stateChanged(DLLState::IsLoaded);

        updateTransformFunc = (UpdateTransformFuncType)lib.resolve(funcName.toUtf8().constData());

        if(updateTransformFunc)
            emit stateChanged(DLLState::IsResolved);
        else
            emit stateChanged(DLLState::FailedToResolve);
    }
    else
    {
        lib.unload();
        updateTransformFunc = nullptr;
        emit stateChanged(DLLState::FailedToLoad);
    }
}

void TransformDllController::unloadDll()
{
    lib.unload();
    updateTransformFunc = nullptr;

    emit stateChanged(DLLState::IsUnloaded);
}











TransformDllControllerSettingWidget::TransformDllControllerSettingWidget(QWidget *parent)
    : QWidget(parent)
    , dllPathEdit(new QLineEdit(this))
    , openDialogButton(new QToolButton(this))
    , fileDialog(new QFileDialog(this))
    , funcNameEdit(new QLineEdit(this))
    , requestLoadButton(new QPushButton("Load", this))
    , requestUnloadButton(new QPushButton("Unload", this))
    , validPalette(dllPathEdit->palette())
    , invalidPalette(dllPathEdit->palette())
{
    QFormLayout *fLayout = new QFormLayout(this);
    QHBoxLayout *dllPathEditLayout = new QHBoxLayout;
    QHBoxLayout *requestButtonLayout = new QHBoxLayout;

    setLayout(fLayout);
    fLayout->addRow("Dll Path", dllPathEditLayout);
    dllPathEditLayout->addWidget(dllPathEdit);
    dllPathEditLayout->addWidget(openDialogButton);
    fLayout->addRow("Func Name", funcNameEdit);
    fLayout->addRow("", requestButtonLayout);
    requestButtonLayout->addWidget(requestLoadButton);
    requestButtonLayout->addWidget(requestUnloadButton);

    openDialogButton->setText("...");
    openDialogButton->setToolTip("open file dialog");
    fileDialog->setFileMode(QFileDialog::FileMode::ExistingFile);
    fileDialog->setNameFilter("*.dll");
    requestUnloadButton->setEnabled(false);

    validPalette.setColor(QPalette::Text, Qt::black);
    invalidPalette.setColor(QPalette::Text, Qt::red);

    fLayout->setContentsMargins(0, 0, 0, 0);
    dllPathEditLayout->setSpacing(0);
    dllPathEditLayout->setContentsMargins(0, 0, 0, 0);
    requestButtonLayout->setSpacing(0);
    requestButtonLayout->setContentsMargins(0, 0, 0, 0);

    connect(dllPathEdit, &QLineEdit::textEdited, this, &TransformDllControllerSettingWidget::dllPathEdited);
    connect(openDialogButton, &QToolButton::released, fileDialog, &QFileDialog::show);
    connect(fileDialog, &QFileDialog::fileSelected, this, &TransformDllControllerSettingWidget::dllPathEdited);
    connect(fileDialog, &QFileDialog::fileSelected, dllPathEdit, &QLineEdit::setText);
    connect(funcNameEdit, &QLineEdit::textEdited, this, &TransformDllControllerSettingWidget::funcNameEdited);
    connect(requestLoadButton, &QPushButton::released, this, &TransformDllControllerSettingWidget::dllLoadRequested);
    connect(requestUnloadButton, &QPushButton::released, this, &TransformDllControllerSettingWidget::dllUnloadRequested);
}

void TransformDllControllerSettingWidget::receiveDllState(const TransformDllController::DLLState& state)
{
    dllPathEdit->setPalette(validPalette);
    funcNameEdit->setPalette(validPalette);
    requestLoadButton->setEnabled(true);
    requestUnloadButton->setEnabled(false);

    switch(state)
    {
    case TransformDllController::DLLState::FailedToLoad:
    {
        dllPathEdit->setPalette(invalidPalette);
        return;
    }
    case TransformDllController::DLLState::FailedToResolve:
    {
        funcNameEdit->setPalette(invalidPalette);
        return;
    }
    case TransformDllController::DLLState::IsLoaded:
    {
        return;
    }
    case TransformDllController::DLLState::IsResolved:
    {
        requestLoadButton->setEnabled(false);
        requestUnloadButton->setEnabled(true);
        return;
    }
    case TransformDllController::DLLState::IsUnloaded:
    {
        requestLoadButton->setEnabled(true);
        requestUnloadButton->setEnabled(false);
        return;
    }
    default:
        return;
    }
}





























