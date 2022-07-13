#include "surfacemesh.h"

SurfaceGeometry::SurfaceGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , normalAttribute(new Qt3DCore::QAttribute(this))
    , colorAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , normalBuffer(new Qt3DCore::QBuffer(this))
    , colorBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
    , gridIndices(new GridIndices(nullptr))
    , gridColorVertex(new GridColorVertex(nullptr))
    , gridNormalVertex(new GridNormalVertex(nullptr))
{
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(positionBuffer);
    positionAttribute->setVertexSize(3); //(x,y,z)
    positionAttribute->setByteStride(3 * sizeof(float)); //(x,y,z) * float

    normalAttribute->setName(Qt3DCore::QAttribute::defaultNormalAttributeName());
    normalAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    normalAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    normalAttribute->setBuffer(normalBuffer);
    normalAttribute->setVertexSize(3); //(nx,ny,nz)
    normalAttribute->setByteStride(3 * sizeof(float)); //(nx,ny,nz) * float

    colorAttribute->setName(Qt3DCore::QAttribute::defaultColorAttributeName());
    colorAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    colorAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    colorAttribute->setBuffer(colorBuffer);
    colorAttribute->setVertexSize(3); //(r,g,b)
    colorAttribute->setByteStride(3 * sizeof(float)); //(r,g,b) * float

    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);

    addAttribute(positionAttribute);
    addAttribute(normalAttribute);
    addAttribute(colorAttribute);
    addAttribute(indexAttribute);

    connect(this, &SurfaceGeometry::gridSizeChanged, gridIndices, &GridIndices::setSize);
    connect(this, &SurfaceGeometry::gridSizeChanged, gridColorVertex, &GridColorVertex::setSize);
    connect(this, &SurfaceGeometry::gridSizeChanged, gridNormalVertex, &GridNormalVertex::setSize);
    connect(this, &SurfaceGeometry::updateIndicesRequested, gridIndices, &GridIndices::createTriangleIndices);
    connect(this, &SurfaceGeometry::updateColorVertexRequested, gridColorVertex, &GridColorVertex::createColorVertex);
    connect(this, &SurfaceGeometry::updateNormalVertexRequested, gridNormalVertex, &GridNormalVertex::createNormalVertex);
    connect(gridIndices, &GridIndices::indicesCreated, this, &SurfaceGeometry::updateIndices);
    connect(gridColorVertex, &GridColorVertex::colorVertexCreated, this, &SurfaceGeometry::updateColorVertex);
    connect(gridNormalVertex, &GridNormalVertex::normalVertexCreated, this, &SurfaceGeometry::updateNormalVertex);

    connect(this, &SurfaceGeometry::colorMapMinValueChanged, gridColorVertex, &GridColorVertex::setMinValue);
    connect(this, &SurfaceGeometry::colorMapMaxValueChanged, gridColorVertex, &GridColorVertex::setMaxValue);
    connect(this, &SurfaceGeometry::colorMapTypeChanged, gridColorVertex, &GridColorVertex::setColorMapType);

    thread.start();
    gridIndices->moveToThread(&thread);
    gridColorVertex->moveToThread(&thread);
    gridNormalVertex->moveToThread(&thread);

    setDefaultSurface();
}

SurfaceGeometry::~SurfaceGeometry()
{
    thread.quit();
    thread.wait();
    gridIndices->deleteLater();
    gridColorVertex->deleteLater();
    gridNormalVertex->deleteLater();
}

void SurfaceGeometry::setDefaultSurface()
{
    //xrange[-50:50], zrange[-50:50]
    constexpr unsigned int rangeX = 2 * 50 + 1;
    constexpr unsigned int rangeZ = 2 * 50 + 1;
    constexpr unsigned int latticeCount = rangeX * rangeZ;
    constexpr float dx = 1.0f;
    constexpr float dz = 1.0f;

    QByteArray positionArray;
    positionArray.resize(3 * latticeCount * sizeof(float));
    float *p = reinterpret_cast<float*>(positionArray.data());

    for(unsigned int iz = 0; iz < rangeZ; ++iz)
        for(unsigned int ix = 0; ix < rangeX; ++ix)
        {
            const float x = ix * dx;
            const float z = iz * dz;
            *p++ = x;
            *p++ = 5.0f * (sin(x / 5.0f) + cos(z / 5.0f));
            *p++ = z;
        }

    setColorMapMinValue(-10.0f);
    setColorMapMaxValue(10.0f);
    setData(positionArray, rangeX, rangeZ);
}

void SurfaceGeometry::setData(const QByteArray &pos, const unsigned int &rowCount, const unsigned int &colCount)
{
    const unsigned int oldLatticeCount = _rowCount * _colCount;
    const unsigned int newLatticeCount = rowCount * colCount;

    if(newLatticeCount > oldLatticeCount)
    {
        positionBuffer->setData(pos);
        positionAttribute->setCount(newLatticeCount);
        _rowCount = rowCount;
        _colCount = colCount;
        emit gridSizeChanged(_rowCount, _colCount);
        emit updateColorVertexRequested(pos);
        emit updateNormalVertexRequested(pos);
        emit updateIndicesRequested();
    }
    else if(newLatticeCount < oldLatticeCount)
    {
        _rowCount = rowCount;
        _colCount = colCount;
        emit gridSizeChanged(_rowCount, _colCount);
        emit updateIndicesRequested();
        emit updateColorVertexRequested(pos);
        emit updateNormalVertexRequested(pos);
    }
    else
    {
        emit updateColorVertexRequested(pos);
        emit updateNormalVertexRequested(pos);
    }
}

void SurfaceGeometry::updateIndices(const QByteArray& indices, const unsigned int& count)
{
    indexBuffer->setData(indices);
    indexAttribute->setCount(count);
}

void SurfaceGeometry::updateColorVertex(const QByteArray& array, const unsigned int& count)
{
    colorBuffer->setData(array);
    colorAttribute->setCount(count);
}

void SurfaceGeometry::updateNormalVertex(const QByteArray& array, const unsigned int& count)
{
    normalBuffer->setData(array);
    normalAttribute->setCount(count);
}










SurfaceMesh::SurfaceMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new SurfaceGeometry(this));

    //setPrimitiveType(Qt3DRender::QGeometryRenderer::)
}







