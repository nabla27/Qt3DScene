#include "gridmeshanimation.h"


const QString GridMeshDllController::sizeFuncName = "sizeUIpUIpF";
const QString GridMeshDllController::dataFuncName = "dataFpF";
GridMeshDllController::GridMeshDllController(GridMesh *mesh, QObject *parent)
    : AbstractController(parent)
    , mesh(mesh)
    , sizeFunc(nullptr)
    , dataFunc(nullptr)
{
    connect(mesh, &GridMesh::destroyed, this, &GridMeshDllController::deleteLater);
}

QWidget* GridMeshDllController::paramWidgets(QWidget *parent) const
{
    DllSelectorWidget *widget = new DllSelectorWidget(QStringList() << sizeFuncName
                                                                    << dataFuncName,
                                                      parent);

    connect(widget, &DllSelectorWidget::dllPathEdited, this, &GridMeshDllController::setDllPath);
    connect(widget, &DllSelectorWidget::funcSymbolEdited, this, &GridMeshDllController::setFuncSymbol);
    connect(widget, &DllSelectorWidget::loadRequested, this, &GridMeshDllController::loadDll);
    connect(widget, &DllSelectorWidget::unloadRequested, this, &GridMeshDllController::unloadDll);
    connect(this, &GridMeshDllController::dllStateChanged, widget, &DllSelectorWidget::receiveDllState);

    return widget;
}

void GridMeshDllController::update(const int& msec)
{
    if(!sizeFunc || !dataFunc) return;

    unsigned int rows = 0, cols = 0;
    sizeFunc(&rows, &cols, (float)msec / 1000.0f);

    if(latticeCount != rows * cols)
    {
        latticeCount = rows * cols;
        array.resize(3 * latticeCount * sizeof(float)); //(x,y,z) * 格子数 * float
    }

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
        sizeFunc = (GridMeshSizeFuncType)lib.resolve((funcSymbol + sizeFuncName).toUtf8().constData());

        if(sizeFunc)
        {
            dataFunc = (GridMeshDataFuncType)lib.resolve((funcSymbol + dataFuncName).toUtf8().constData());

            if(dataFunc)
            {
                emit dllStateChanged(DllSelectorWidget::DllState::Resolved);
                return;
            }
            else
                emit dllStateChanged(DllSelectorWidget::DllState::FailedToResolve);
        }
        else
        {
            emit dllStateChanged(DllSelectorWidget::DllState::FailedToResolve);
        }
    }
    else
    {
        emit dllStateChanged(DllSelectorWidget::DllState::FailedToLoad);
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

    emit dllStateChanged(DllSelectorWidget::DllState::Unloaded);
}





