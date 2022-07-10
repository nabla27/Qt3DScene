#include "potential3dgridmesh.h"

Contour3DGridGeometry::Contour3DGridGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
{
    positionAttribute->setName(Qt3DCore::QAttribute::defaultColorAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(positionBuffer);
    positionAttribute->setVertexSize(3); //(x,y,z)
    positionAttribute->setByteStride(4 * sizeof(float)); //(x,y,z,v)

    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);

    addAttribute(positionAttribute);
    addAttribute(indexAttribute);
}

Contour3DGridGeometry::~Contour3DGridGeometry()
{

}

void Contour3DGridGeometry::setDefaultGrid()
{
    constexpr unsigned int rangeX = 2 * 50 + 1;
    constexpr unsigned int rangeY = 2 * 50 + 1;
    constexpr unsigned int rangeZ = 2 * 50 + 1;
    constexpr float dx = 1.0f;
    constexpr float dy = 1.0f;
    constexpr float dz = 1.0f;

    QByteArray positionArray;
    positionArray.resize(4 * rangeX * rangeY * rangeZ * sizeof(float)); //(x,y,z,v) * 格子数 * float
    float *p = reinterpret_cast<float*>(positionArray.data());

    for(unsigned int ix = 0; ix < rangeX; ++ix)
    {
        const float x = dx * ix;
        for(unsigned int iz = 0; iz < rangeZ; ++iz)
        {
            const float z = dz * iz;
            for(unsigned int iy = 0; iy < rangeY; ++iy)
            {
                const float y = dy * iy;

                *p++ = x;
                *p++ = y;
                *p++ = z;
                *p++ = x * x + y * y + z * z;
            }
        }
    }

    setData(positionArray, rangeX, rangeY, rangeZ);
}

void Contour3DGridGeometry::setData(const QByteArray &pos, const unsigned int &rangeX, const unsigned int &rangeY, const unsigned int &rangeZ)
{
    positionBuffer->setData(pos);
    positionAttribute->setCount(rangeX * rangeY * rangeZ);

    range_x = rangeX;
    range_y = rangeY;
    range_z = rangeZ;

    emit updateIndicesRequested(pos, rangeX, rangeY, rangeZ, potential, threshold);
}

void Contour3DGridGeometry::updateIndices(const QByteArray& indices, const unsigned int& count)
{
    indexBuffer->setData(indices);
    indexAttribute->setCount(count);
}








Contour3DGridMesh::Contour3DGridMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new Contour3DGridGeometry(this));

    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}






