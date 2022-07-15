#ifndef SURFACEMESHANIMATION_H
#define SURFACEMESHANIMATION_H

#include <QMutex>
#include <QLibrary>
#include "animation.h"
#include "../custommesh/surfacemesh.h"
#include "animationsetting.h"
#include "../layout.h"


class AbstractDllController : public AbstractController
{
    Q_OBJECT

public:
    explicit AbstractDllController(QObject *parent);

public:
    QWidget* paramWidgets(QWidget *parent) const override;
    bool isResolved() const { return isResolve; }

public slots:
    virtual void update(const int& msec) override = 0;

    void setDllPath(const QString& path) { dllPath = path; }
    void setFuncSymbol(const QString& symbol) { funcSymbol = symbol; }
    void loadDll();
    void unloadDll();

protected:
    struct DllFuncInfo
    {
        DllFuncInfo(const QString& funcName) : funcName(funcName), funcPtr(nullptr) {}
        QString funcName = QString();
        void* funcPtr = nullptr;
    };
    QList<DllFuncInfo> funcInfoList = QList<DllFuncInfo>();

private:
    QLibrary lib;
    bool isResolve = false;

    QString dllPath;
    QString funcSymbol;

signals:
    void dllStateChanged(const DllSelectorWidget::DllState& state);
};











class SurfaceMeshDllController : public AbstractDllController
{
    Q_OBJECT

    typedef void(*SurfaceMeshSizeFuncType)(unsigned int*, unsigned int*, float);
    typedef void(*SurfaceMeshDataFuncType)(float*, float);

public:
    explicit SurfaceMeshDllController(SurfaceMesh *mesh, QObject *parent = nullptr);

public slots:
    void update(const int& msec) override;

private:
    SurfaceMesh *mesh;
    QMutex mutex;

    QByteArray array;
    unsigned int latticeCount = 0;
};

#endif // SURFACEMESHANIMATION_H
