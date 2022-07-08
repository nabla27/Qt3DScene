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
    void setSizeFuncName(const QString& name) { sizeFuncName = name; }
    void setDataFuncName(const QString& name) { dataFuncName = name; }
    void loadDll();
    void unloadDll();

private:
    GridMesh *mesh;
    GridMeshSizeFuncType sizeFunc;
    GridMeshDataFuncType dataFunc;
    QMutex mutex;
    QLibrary lib;
    QString dllPath;
    QString sizeFuncName;
    QString dataFuncName;

signals:
    void dllStateChanged(const GridMeshDataDllSelector::DllState& state);
};

#endif // GRIDMESHANIMATION_H
