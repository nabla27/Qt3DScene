#ifndef TRANSFORMANIMATION_H
#define TRANSFORMANIMATION_H

#include "animation.h"
#include "src/components-setting/animation/animationsetting.h"
#include <Qt3DCore/QTransform>
#include <QThread>
#include <QMutex>
#include <QLibrary>

class QPushButton;
class QFileDialog;




class TransformDllController : public AbstractController
{
    Q_OBJECT

    typedef void (*UpdateTransformFuncType)(float*) ;

public:
    explicit TransformDllController(Qt3DCore::QTransform *transform, QObject *parent = nullptr);

    enum class DLLState { IsLoaded, IsResolved, FailedToLoad, FailedToResolve, IsUnloaded };
    Q_ENUM(DLLState)

public:
    QWidget* paramWidgets(QWidget *parent) const override;

public slots:
    void update(const int& msec) override;
    void setDllPath(const QString& path);
    void setFuncName(const QString& name);
    void loadDll();
    void unloadDll();

private:
    Qt3DCore::QTransform *transform;
    UpdateTransformFuncType updateTransformFunc;
    QMutex mutex;
    QLibrary lib;
    QString funcName;
    QString dllPath;

signals:
    void stateChanged(const TransformDllController::DLLState& state);
};

class TransformDllControllerSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransformDllControllerSettingWidget(QWidget *parent);

public slots:
    void receiveDllState(const TransformDllController::DLLState& state);

private:
    QLineEdit *dllPathEdit;
    QToolButton *openDialogButton;
    QFileDialog *fileDialog;
    QLineEdit *funcNameEdit;
    QPushButton *requestLoadButton;
    QPushButton *requestUnloadButton;

    QPalette validPalette;
    QPalette invalidPalette;

signals:
    void dllPathEdited(const QString& path);
    void funcNameEdited(const QString& name);
    void dllLoadRequested();
    void dllUnloadRequested();
};


















#endif // TRANSFORMANIMATION_H
