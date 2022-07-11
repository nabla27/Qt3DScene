#include "explicitfuncmesh.h"

ExplicitFuncGeometry::ExplicitFuncGeometry(Qt3DCore::QNode *parent)
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
    positionAttribute->setVertexSize(3);
    positionAttribute->setByteStride(3 * sizeof(float)); //(x,y,z) * float

    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);

    addAttribute(positionAttribute);
    addAttribute(indexAttribute);

    connect(this, &ExplicitFuncGeometry::updateIndicesRequested, gridIndices, &GridIndices::createSingleLineIndices);
    connect(gridIndices, &GridIndices::created, this, &ExplicitFuncGeometry::updateIndices);

    thread.start();
    gridIndices->moveToThread(&thread);

    setDefaultFunc();
}

ExplicitFuncGeometry::~ExplicitFuncGeometry()
{
    thread.quit();
    thread.wait();
}

void ExplicitFuncGeometry::setDefaultFunc()
{
    constexpr unsigned int samples = 1000;
    constexpr float dx = 0.1f;

    QByteArray positionArray;
    positionArray.resize(3 * samples * sizeof(float)); //(x,y,z) * samples * float
    float *p = reinterpret_cast<float*>(positionArray.data());

    for(unsigned int ix = 0; ix < samples; ++ix)
    {
        const float x = ix * dx;
        *p++ = x;
        *p++ = sin(x);
        *p++ = 0.0f;
    }

    setData(positionArray, samples);
}

void ExplicitFuncGeometry::setData(const QByteArray &pos, const unsigned int &samples)
{
    positionBuffer->setData(pos);
    positionAttribute->setCount(samples);

    if(this->samples != samples)
    {
        this->samples = samples;
        emit updateIndicesRequested(samples);
    }
}

void ExplicitFuncGeometry::updateIndices(const QByteArray& indices, const unsigned int& indexCount)
{
    indexBuffer->setData(indices);
    indexAttribute->setCount(indexCount);
}







ExplicitFuncMesh::ExplicitFuncMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new ExplicitFuncGeometry(this));

    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}






#include <QFormLayout>
#include <QComboBox>
ExplicitFuncMeshSettingWidget::ExplicitFuncMeshSettingWidget(ExplicitFuncMesh *mesh, QWidget *parent)
    : AbstractComponentsSettingWidget(mesh, "ExplicitFunc", parent)
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

    connect(dataTypeCombo, &QComboBox::currentIndexChanged, this, &ExplicitFuncMeshSettingWidget::setDataSelector);

    thread.start();
}

ExplicitFuncMeshSettingWidget::~ExplicitFuncMeshSettingWidget()
{
    thread.quit();
    thread.wait();
}

void ExplicitFuncMeshSettingWidget::setDataSelector(const int index)
{
    if(selectorSettingWidget) selectorSettingWidget->deleteLater();
    AbstractExplicitFuncMeshDataSelector *selector = nullptr;

    switch(DataType(index))
    {
    case DataType::Default:
    {
        mesh->setDefaultFunc();
        selectorSettingWidget = nullptr;
        return;
    }
    case DataType::DLL:
    {
        selector = new ExplicitFuncMeshDataDllSelector(nullptr);
        selectorSettingWidget = selector->settingWidget(contents);
        break;
    }
    case DataType::Csv:
    case DataType::Tsv:
    case DataType::MathExp:
    default:
        return;
    }

    connect(selector, &AbstractExplicitFuncMeshDataSelector::dataSelected, mesh, &ExplicitFuncMesh::setData);
    connect(selectorSettingWidget, &QWidget::destroyed, selector, &AbstractExplicitFuncMeshDataSelector::deleteLater);

    selector->moveToThread(&thread);
    vLayout->addWidget(selectorSettingWidget);
}








const QString ExplicitFuncMeshDataDllSelector::samplesFuncName = "samplesUIp";
const QString ExplicitFuncMeshDataDllSelector::dataFuncName = "dataFp";
ExplicitFuncMeshDataDllSelector::ExplicitFuncMeshDataDllSelector(QObject *parent)
    : AbstractExplicitFuncMeshDataSelector(parent)
    , samplesFunc(nullptr)
    , dataFunc(nullptr)
{
}

void ExplicitFuncMeshDataDllSelector::load()
{
    lib.setFileName(dllPath);

    if(lib.load())
    {
        samplesFunc = (SamplesFuncType)lib.resolve((funcSymbol + samplesFuncName).toUtf8().constData());

        if(samplesFunc)
        {
            dataFunc = (DataFuncType)lib.resolve((funcSymbol + dataFuncName).toUtf8().constData());

            if(dataFunc)
            {
                unsigned int samples = 0;
                samplesFunc(&samples);

                if(prevSamples != samples)
                {
                    prevSamples = samples;
                    array.resize(3 * samples * sizeof(float)); //(x,y,z) * samples * float
                }

                dataFunc(reinterpret_cast<float*>(array.data()));

                emit dllStateChanged(DllSelectorWidget::DllState::Resolved);
                emit dataSelected(array, samples);
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

void ExplicitFuncMeshDataDllSelector::unload()
{
    lib.unload();
    samplesFunc = nullptr;
    dataFunc = nullptr;
    emit dllStateChanged(DllSelectorWidget::DllState::Unloaded);
}

QWidget *const ExplicitFuncMeshDataDllSelector::settingWidget(QWidget *parent) const
{
    DllSelectorWidget *widget = new DllSelectorWidget(QStringList() << samplesFuncName
                                                                    << dataFuncName,
                                                      parent);

    connect(widget, &DllSelectorWidget::dllPathEdited, this, &ExplicitFuncMeshDataDllSelector::setDllPath);
    connect(widget, &DllSelectorWidget::funcSymbolEdited, this, &ExplicitFuncMeshDataDllSelector::setFuncSymbol);
    connect(widget, &DllSelectorWidget::loadRequested, this, &ExplicitFuncMeshDataDllSelector::load);
    connect(widget, &DllSelectorWidget::unloadRequested, this, &ExplicitFuncMeshDataDllSelector::unload);
    connect(this, &ExplicitFuncMeshDataDllSelector::dllStateChanged, widget, &DllSelectorWidget::receiveDllState);

    return widget;
}



