#include "gridmeshanimation.h"

GridMeshDllController::GridMeshDllController(GridMesh *mesh, QObject *parent)
    : AbstractController(parent)
    , mesh(mesh)
    , sizeFunc(nullptr)
    , dataFunc(nullptr)
    , sizeFuncName(QString())
    , dataFuncName(QString())
{
    connect(mesh, &GridMesh::destroyed, this, &GridMeshDllController::deleteLater);
}

QWidget* GridMeshDllController::paramWidgets(QWidget *parent) const
{
    GridMeshDataDllSelectorSettingWidget *widget = new GridMeshDataDllSelectorSettingWidget(parent);

    connect(widget, &GridMeshDataDllSelectorSettingWidget::dllPathEdited, this, &GridMeshDllController::setDllPath);
    connect(widget, &GridMeshDataDllSelectorSettingWidget::sizeFuncNameEdited, this, &GridMeshDllController::setSizeFuncName);
    connect(widget, &GridMeshDataDllSelectorSettingWidget::dataFuncNameEdited, this, &GridMeshDllController::setDataFuncName);
    connect(widget, &GridMeshDataDllSelectorSettingWidget::loadRequested, this, &GridMeshDllController::loadDll);
    connect(widget, &GridMeshDataDllSelectorSettingWidget::unloadRequested, this, &GridMeshDllController::unloadDll);
    connect(this, &GridMeshDllController::dllStateChanged, widget, &GridMeshDataDllSelectorSettingWidget::receiveDllState);

    return widget;
}

void GridMeshDllController::update(const int& msec)
{
    if(!sizeFunc || !dataFunc) return;

    unsigned int rows = 0, cols = 0;
    sizeFunc(&rows, &cols, (float)msec / 1000.0f);

    QByteArray array;
    array.resize(3 * rows * cols * sizeof(float)); //(x,y,z) * 格子数 * float
    float *p = reinterpret_cast<float*>(array.data());

    dataFunc(p, (float)msec / 1000.0f);

    mutex.lock();

    mesh->setData(array, rows, cols);

    mutex.unlock();

    emit updated(msec);
}

void GridMeshDllController::loadDll()
{
    lib.setFileName(dllPath);

    if(lib.load())
    {
        sizeFunc = (GridMeshSizeFuncType)lib.resolve(sizeFuncName.toUtf8().constData());

        if(sizeFunc)
        {
            dataFunc = (GridMeshDataFuncType)lib.resolve(dataFuncName.toUtf8().constData());

            if(dataFunc)
            {
                emit dllStateChanged(GridMeshDataDllSelector::DllState::Resolved);
                return;
            }
            else
                emit dllStateChanged(GridMeshDataDllSelector::DllState::FailedToResolveData);
        }
        else
        {
            emit dllStateChanged(GridMeshDataDllSelector::DllState::FailedToResolveSize);
        }
    }
    else
    {
        emit dllStateChanged(GridMeshDataDllSelector::DllState::FailedToLoad);
    }

    lib.unload();
    sizeFunc = nullptr;
    dataFunc = nullptr;
}

void GridMeshDllController::unloadDll()
{
    lib.unload();
    sizeFunc = nullptr;
    dataFunc = nullptr;

    emit dllStateChanged(GridMeshDataDllSelector::DllState::Unloaded);
}





