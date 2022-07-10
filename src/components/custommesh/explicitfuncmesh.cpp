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



