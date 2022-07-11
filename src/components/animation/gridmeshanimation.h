#ifndef GRIDMESHANIMATION_H
#define GRIDMESHANIMATION_H

#include <QMutex>
#include "animation.h"
#include "../custommesh/gridmesh.h"




class GridMeshDllController : public AbstractController
{
    Q_OBJECT

    typedef void(*GridMeshSizeFuncType)(unsigned int*, unsigned int*, float);
    typedef void(*GridMeshDataFuncType)(float*, float);

public:
    explicit GridMeshDllController(GridMesh *mesh, QObject *parent = nullptr);

public:
    QWidget* paramWidgets(QWidget *parent) const override;

public slots:
    void update(const int& msec) override;
    void setDllPath(const QString& path) { dllPath = path; }
    void setFuncSymbol(const QString& symbol) { funcSymbol = symbol; }
    void loadDll();
    void unloadDll();

private:
    GridMesh *mesh;
    QMutex mutex;

    QLibrary lib;
    GridMeshSizeFuncType sizeFunc;
    GridMeshDataFuncType dataFunc;

    QString dllPath;
    QString funcSymbol;
    static const QString sizeFuncName;
    static const QString dataFuncName;

    QByteArray array;
    unsigned int latticeCount = 0;

signals:
    void dllStateChanged(const DllSelectorWidget::DllState& state);
};

#endif // GRIDMESHANIMATION_H
