#include "vector3dmesh.h"







void Vector3DVertex::createVertex(const QByteArray& vec)
{

}

void Vector3DVertex::setRange(const unsigned int& range)
{

}

void Vector3DVertex::setDelta(const float& dx)
{

}

void Vector3DVertex::setGrid(const unsigned int& range, const float& dx)
{

}










Vector3DGeometry::Vector3DGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
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

    thread.start();

    setDefaultVector();
}

Vector3DGeometry::~Vector3DGeometry()
{
    thread.quit();
    thread.wait();
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

    constexpr float dx = 1.0f;
    setData(vec, range, dx);
}

void Vector3DGeometry::setData(const QByteArray &vec, const unsigned int &range, const float &dx)
{

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

    //setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}
