#include "gridmesh.h"

GridGeometry::GridGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
    , gridIndices(new GridIndices(nullptr))
{
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(positionBuffer);
    positionAttribute->setVertexSize(3); //(x,y,z)
    positionAttribute->setByteStride(3 * sizeof(float)); //(x,y,z) * float

    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);

    addAttribute(positionAttribute);
    addAttribute(indexAttribute);

    connect(this, &GridGeometry::updateIndicesRequested, gridIndices, &GridIndices::createSquareIndices);
    connect(gridIndices, &GridIndices::created, this, &GridGeometry::updateIndices);

    thread.start();
    gridIndices->moveToThread(&thread);

    setDefaultGrid();
}

GridGeometry::~GridGeometry()
{
    thread.quit();
    thread.wait();
}

void GridGeometry::setDefaultGrid()
{
    //xrange[-50:50], zrange[-50:50]
    constexpr unsigned int initRows = 2 * 50 + 1;
    constexpr unsigned int initCols = 2 * 50 + 1;
    constexpr unsigned int gridCount = initRows * initCols;
    constexpr float dx = 1.0f;
    constexpr float dz = 1.0f;

    QByteArray positionArray;
    positionArray.resize(3 * gridCount * sizeof(float)); //(x,y,z) * 格子数 * float
    float *p = reinterpret_cast<float*>(positionArray.data());

    for(unsigned int row = 0; row < initRows; ++row)
        for(unsigned int col = 0; col < initCols; ++col)
        {
            *p++ = dx * row;
            *p++ = 0;
            *p++ = dz * col;
        }

    setData(positionArray, initRows, initCols);
}

void GridGeometry::setData(const QByteArray &pos, const unsigned int& rowCount, const unsigned int& colCount)
{
    positionBuffer->setData(pos);
    positionAttribute->setCount(rowCount * colCount);

    if(_rowCount != rowCount || _colCount != colCount)
    {
        _rowCount = rowCount;
        _colCount = colCount;
        emit updateIndicesRequested(_rowCount, _colCount);
    }
}

void GridGeometry::updateIndices(const QByteArray& indices, const unsigned int& count)
{
    indexBuffer->setData(indices);
    indexAttribute->setCount(count);
}







GridMesh::GridMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new GridGeometry(this));

    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}










#include <QFormLayout>
#include <QComboBox>
GridMeshSettingWidget::GridMeshSettingWidget(GridMesh *mesh, QWidget *parent)
    : AbstractComponentsSettingWidget(mesh, "GridMesh", parent)
    , mesh(mesh)
    , vLayout(new QVBoxLayout(contents))
    , selectorSettingWidget(nullptr)
{
    QFormLayout *fLayout = new QFormLayout;
    QComboBox *dataTypeCombo = new QComboBox(contents);

    contents->setLayout(vLayout);
    vLayout->addLayout(fLayout);
    fLayout->addRow("Data Type", dataTypeCombo);

    dataTypeCombo->addItems(enumToStrings(DataType(0)));

    connect(dataTypeCombo, &QComboBox::currentIndexChanged, this, &GridMeshSettingWidget::setDataSelector);
}

void GridMeshSettingWidget::setDataSelector(const int index)
{
    if(selectorSettingWidget) selectorSettingWidget->deleteLater();
    AbstractGridMeshDataSelector *selector = nullptr;

    switch(GridMeshSettingWidget::DataType(index))
    {
    case DataType::Default:
    {
        mesh->setDefaultGrid();
        selectorSettingWidget = nullptr;
        return;
    }
    case DataType::DLL:
    {
        selector = new GridMeshDataDllSelector(nullptr);
        selectorSettingWidget = selector->settingWidget(contents);
        break;
    }
    default:
        return;
    }

    connect(selector, &AbstractGridMeshDataSelector::dataSelected, mesh, &GridMesh::setData);
    connect(selectorSettingWidget, &QWidget::destroyed, selector, &AbstractGridMeshDataSelector::deleteLater);

    vLayout->addWidget(selectorSettingWidget);
}













const QString GridMeshDataDllSelector::sizeFuncName = "sizeUIpUIp";
const QString GridMeshDataDllSelector::dataFuncName = "dataFp";
GridMeshDataDllSelector::GridMeshDataDllSelector(QObject *parent)
    : AbstractGridMeshDataSelector(parent)
    , sizeFunc(nullptr)
    , dataFunc(nullptr)
{

}

void GridMeshDataDllSelector::load()
{
    lib.setFileName(dllPath);

    if(lib.load())
    {
        sizeFunc = (SizeFuncType)lib.resolve((funcSymbol + sizeFuncName).toUtf8().constData());

        if(sizeFunc)
        {
            dataFunc = (DataFuncType)lib.resolve((funcSymbol + dataFuncName).toUtf8().constData());

            if(dataFunc)
            {
                unsigned int rows = 0, cols = 0;
                sizeFunc(&rows, &cols);

                QByteArray array;
                array.resize(3 * rows * cols * sizeof(float));
                dataFunc(reinterpret_cast<float*>(array.data()));

                emit dllStateChanged(DllSelectorWidget::DllState::Resolved);
                emit dataSelected(array, rows, cols);
            }
            else
            {
                lib.unload();
                emit dllStateChanged(DllSelectorWidget::DllState::FailedToResolve);
            }
        }
        else
        {
            lib.unload();
            emit dllStateChanged(DllSelectorWidget::DllState::FailedToResolve);
        }
    }
    else
    {
        lib.unload();
        emit dllStateChanged(DllSelectorWidget::DllState::FailedToLoad);
    }
}

void GridMeshDataDllSelector::unload()
{
    lib.unload();
    sizeFunc = nullptr;
    dataFunc = nullptr;
    emit dllStateChanged(DllSelectorWidget::DllState::Unloaded);
}

QWidget *const GridMeshDataDllSelector::settingWidget(QWidget *parent) const
{
    DllSelectorWidget *widget = new DllSelectorWidget(QStringList() << sizeFuncName
                                                                    << dataFuncName,
                                                      parent);

    connect(widget, &DllSelectorWidget::dllPathEdited, this, &GridMeshDataDllSelector::setDllPath);
    connect(widget, &DllSelectorWidget::funcSymbolEdited, this, &GridMeshDataDllSelector::setFuncSymbol);
    connect(widget, &DllSelectorWidget::loadRequested, this, &GridMeshDataDllSelector::load);
    connect(widget, &DllSelectorWidget::unloadRequested, this, &GridMeshDataDllSelector::unload);
    connect(this, &GridMeshDataDllSelector::dllStateChanged, widget, &DllSelectorWidget::receiveDllState);

    return widget;
}





