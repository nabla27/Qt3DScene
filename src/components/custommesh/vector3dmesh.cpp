#include "vector3dmesh.h"







void Vector3DVertex::createVertex(const QByteArray& vec)
{
    const float *v = reinterpret_cast<const float*>(vec.data());
    float *p = reinterpret_cast<float*>(vertices.data());

    for(unsigned int iy = 0; iy < _range; ++iy)
        for(unsigned int iz = 0; iz < _range; ++iz)
            for(unsigned int ix = 0; ix < _range; ++ix)
            {
                const float x = ix * _dx;
                const float y = iy * _dx;
                const float z = iz * _dx;

                const float vx = x + _factor * *v++;
                const float vy = y + _factor * *v++;
                const float vz = z + _factor * *v++;

                //origin
                *p++ = x;
                *p++ = y;
                *p++ = z;

                //direction
                *p++ = vx;
                *p++ = vy;
                *p++ = vz;
            }

    emit vertexCreated(vertices, 3 * _range * _range * _range * 2);
    vector = vec;
}

void Vector3DVertex::updateVertex()
{
    createVertex(vector);
}

void Vector3DVertex::createIndices()
{
    unsigned int *index = reinterpret_cast<unsigned int*>(indices.data());

    const unsigned int indexCount = _range * _range * _range * 2;

    for(unsigned int i = 0; i < indexCount; ++i)
        *index++ = i;

    emit indicesCreated(indices, indexCount);
}

void Vector3DVertex::setRange(const unsigned int& range)
{
    _range = range;
    vertices.resize(3 * _range * _range * _range * 2 * sizeof(float)); //(x,y,z) * range * axis * float
    indices.resize(_range * _range * _range * 2 * sizeof(unsigned int));
}

void Vector3DVertex::setDelta(const float& dx)
{
    _dx = dx;
}

void Vector3DVertex::setFactor(const float& factor)
{
    _factor = factor;
}

void Vector3DVertex::setGrid(const unsigned int& range, const float& dx)
{
    setRange(range);
    setDelta(dx);
}










Vector3DGeometry::Vector3DGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
    , vectorVertex(new Vector3DVertex(nullptr))
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
    indexAttribute->setByteStride(2 * sizeof(unsigned int));

    addAttribute(positionAttribute);
    addAttribute(indexAttribute);

    connect(this, &Vector3DGeometry::updateIndicesRequested, vectorVertex, &Vector3DVertex::createIndices);
    connect(this, &Vector3DGeometry::createVectorRequested, vectorVertex, &Vector3DVertex::createVertex);
    connect(this, &Vector3DGeometry::updateVectorRequested, vectorVertex, &Vector3DVertex::updateVertex);
    connect(this, &Vector3DGeometry::rangeChanged, vectorVertex, &Vector3DVertex::setRange);
    connect(this, &Vector3DGeometry::deltaChanged, vectorVertex, &Vector3DVertex::setDelta);
    connect(this, &Vector3DGeometry::gridChanged, vectorVertex, &Vector3DVertex::setGrid);
    connect(this, &Vector3DGeometry::factorChanged, vectorVertex, &Vector3DVertex::setFactor);
    connect(vectorVertex, &Vector3DVertex::indicesCreated, this, &Vector3DGeometry::updateIndices);
    connect(vectorVertex, &Vector3DVertex::vertexCreated, this, &Vector3DGeometry::updatePosition);

    thread.start();
    vectorVertex->moveToThread(&thread);

    setDefaultVector();
}

Vector3DGeometry::~Vector3DGeometry()
{
    thread.quit();
    thread.wait();
    vectorVertex->deleteLater();
}

void Vector3DGeometry::setDefaultVector()
{
    constexpr unsigned int range = 3;

    QByteArray vec;
    vec.resize(3 * range * range * range * sizeof(float));
    float *p = reinterpret_cast<float*>(vec.data());

    for(unsigned int iy = 0; iy < range; ++iy)
        for(unsigned int iz = 0; iz < range; ++iz)
            for(unsigned int ix = 0; ix < range; ++ix)
            {
                *p++ = ix;
                *p++ = iy;
                *p++ = iz;
            }

    emit deltaChanged(1.0f);
    emit factorChanged(0.1f);
    setData(vec, range);
}

void Vector3DGeometry::setData(const QByteArray &vec, const unsigned int &range)
{
    if(range != _range)
    {
        _range = range;
        emit rangeChanged(_range);

        if(range > _range)
        {
            emit createVectorRequested(vec);
            emit updateIndicesRequested();
        }
        else
        {
            emit updateIndicesRequested();
            emit createVectorRequested(vec);
        }
    }
    else
    {
        emit createVectorRequested(vec);
    }
}

void Vector3DGeometry::setDelta(const float &dx)
{
    if(!qFuzzyCompare(dx, _dx))
    {
        _dx = dx;

        emit deltaChanged(_dx);
        emit updateVectorRequested();
    }
}

void Vector3DGeometry::setFactor(const float &factor)
{
    if(!qFuzzyCompare(_factor, factor))
    {
        _factor = factor;

        emit factorChanged(_factor);
        emit updateVectorRequested();
    }
}

void Vector3DGeometry::updatePosition(const QByteArray& pos, const unsigned int& count)
{
    positionBuffer->setData(pos);
    positionAttribute->setCount(count);
}

void Vector3DGeometry::updateIndices(const QByteArray& indices, const unsigned int& count)
{
    indexBuffer->setData(indices);
    indexAttribute->setCount(count);
}








Vector3DMesh::Vector3DMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new Vector3DGeometry(this));

    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}










#include <QFormLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
Vector3DMeshSettingWidget::Vector3DMeshSettingWidget(Vector3DMesh *mesh, QWidget *parent)
    : AbstractComponentsSettingWidget(mesh, "Vector3D", parent)
    , mesh(mesh)
    , vLayout(new QVBoxLayout(contents))
    , selectorSettingWidget(nullptr)
{
    QFormLayout *fLayout = new QFormLayout;
    QComboBox *dataTypeCombo = new QComboBox(contents);
    QDoubleSpinBox *deltaSpinBox = new QDoubleSpinBox(contents);
    QDoubleSpinBox *factorSpinBox = new QDoubleSpinBox(contents);

    contents->setLayout(vLayout);
    vLayout->addLayout(fLayout);
    fLayout->addRow("Grid Delta", deltaSpinBox);
    fLayout->addRow("Vector Factor", factorSpinBox);
    fLayout->addRow("Data Type", dataTypeCombo);

    constexpr double spinBoxMax = 1000.0;
    deltaSpinBox->setMaximum(spinBoxMax);
    factorSpinBox->setMaximum(spinBoxMax);
    dataTypeCombo->addItems(enumToStrings(DataType(0)));

    deltaSpinBox->setValue(mesh->delta());
    factorSpinBox->setValue(mesh->factor());

    connect(deltaSpinBox, &QDoubleSpinBox::valueChanged, mesh, &Vector3DMesh::setDelta);
    connect(factorSpinBox, &QDoubleSpinBox::valueChanged, mesh, &Vector3DMesh::setFactor);
    connect(static_cast<Vector3DGeometry*>(mesh->geometry()), &Vector3DGeometry::deltaChanged, deltaSpinBox, &QDoubleSpinBox::setValue);
    connect(static_cast<Vector3DGeometry*>(mesh->geometry()), &Vector3DGeometry::factorChanged, factorSpinBox, &QDoubleSpinBox::setValue);
    connect(dataTypeCombo, &QComboBox::currentIndexChanged, this, &Vector3DMeshSettingWidget::setDataSelector);
}

void Vector3DMeshSettingWidget::setDataSelector(const int index)
{

}








