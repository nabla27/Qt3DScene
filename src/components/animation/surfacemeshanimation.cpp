#include "surfacemeshanimation.h"



AbstractDllController::AbstractDllController(QObject *parent)
    : AbstractController(parent)
{
}

QWidget* AbstractDllController::paramWidgets(QWidget *parent) const
{
    QStringList funcNameList;
    for(const DllFuncInfo& info : funcInfoList)
        funcNameList << info.funcName;

    DllSelectorWidget *widget = new DllSelectorWidget(funcNameList, parent);

    connect(widget, &DllSelectorWidget::dllPathEdited, this, &AbstractDllController::setDllPath);
    connect(widget, &DllSelectorWidget::funcSymbolEdited, this, &AbstractDllController::setFuncSymbol);
    connect(widget, &DllSelectorWidget::loadRequested, this, &AbstractDllController::loadDll);
    connect(widget, &DllSelectorWidget::unloadRequested, this, &AbstractDllController::unloadDll);
    connect(this, &AbstractDllController::dllStateChanged, widget, &DllSelectorWidget::receiveDllState);

    return widget;
}

void AbstractDllController::loadDll()
{
    lib.setFileName(dllPath);

    if(lib.load())
    {
        for(DllFuncInfo& info : funcInfoList)
        {
            info.funcPtr = (void*)lib.resolve((funcSymbol + info.funcName).toUtf8().constData());

            /* 関数シンボルの未解決 */
            if(!info.funcPtr)
            {
                lib.unload();
                for(DllFuncInfo& info : funcInfoList)
                    info.funcPtr = nullptr;

                emit dllStateChanged(DllSelectorWidget::DllState::FailedToResolve);
                isResolve = false;
                return;
            }
        }

        /* すべての関数シンボルを解決 */
        emit dllStateChanged(DllSelectorWidget::DllState::Resolved);
        isResolve = true;
    }
    else
    {
        /* DLLファイルのロード失敗 */
        lib.unload();
        for(DllFuncInfo& info : funcInfoList)
            info.funcPtr = nullptr;

        emit dllStateChanged(DllSelectorWidget::DllState::FailedToLoad);
        isResolve = false;
    }
}

void AbstractDllController::unloadDll()
{
    if(lib.unload())
    {
        for(DllFuncInfo& info : funcInfoList)
            info.funcPtr = nullptr;

        emit dllStateChanged(DllSelectorWidget::DllState::Unloaded);
    }
}






SurfaceMeshDllController::SurfaceMeshDllController(SurfaceMesh *mesh, QObject *parent)
    : AbstractDllController(parent)
    , mesh(mesh)
{
    connect(mesh, &SurfaceMesh::destroyed, this, &SurfaceMeshDllController::deleteLater);

    funcInfoList.append(DllFuncInfo("sizeUIpUIpF"));
    funcInfoList.append(DllFuncInfo("dataFpF"));
}

void SurfaceMeshDllController::update(const int& msec)
{
    if(!isResolved()) return;

    const float time = (float)msec / 1000.0f;

    unsigned int rows = 0, cols = 0;
    SurfaceMeshSizeFuncType sizeFunc = static_cast<SurfaceMeshSizeFuncType>(funcInfoList.at(0).funcPtr);
    sizeFunc(&rows, &cols, time);

    if(latticeCount != rows * cols)
    {
        latticeCount = rows * cols;
        array.resize(3 * latticeCount * sizeof(float));
    }

    float *p = reinterpret_cast<float*>(array.data());

    SurfaceMeshDataFuncType dataFunc = static_cast<SurfaceMeshDataFuncType>(funcInfoList.at(1).funcPtr);
    dataFunc(p, time);

    mutex.lock();

    mesh->setData(array, rows, cols);

    mutex.unlock();

    emit updated(msec);
}


