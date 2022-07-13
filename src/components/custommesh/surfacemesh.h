#ifndef SURFACEMESH_H
#define SURFACEMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include <QThread>
#include "custommesh.h"



class SurfaceGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT
public:
    SurfaceGeometry(Qt3DCore::QNode *parent = nullptr);
    ~SurfaceGeometry();

public:
    void setDefaultSurface();
    void setData(const QByteArray& pos, const unsigned int& rowCount, const unsigned int& colCount);
    void setContourMaxValue(const float& value) { emit contourMaxValueChanged(value); }
    void setContourMinValue(const float& value) { emit contourMinValueChanged(value); }

private slots:
    void updateIndices(const QByteArray& indices, const unsigned int& count);
    void updateColorVertex(const QByteArray& vertex, const unsigned int& count);
    void updateNormalVertex(const QByteArray& vertex, const unsigned int& count);

private:
    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *normalAttribute;
    Qt3DCore::QAttribute *colorAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *normalBuffer;
    Qt3DCore::QBuffer *colorBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QThread thread;
    GridIndices *gridIndices;
    GridColorVertex *gridColorVertex;
    GridNormalVertex *gridNormalVertex;

    unsigned int _rowCount = 0;
    unsigned int _colCount = 0;

signals:
    void gridSizeChanged(const unsigned int& rowCount, const unsigned int& colCount);
    void updateIndicesRequested();
    void updateColorVertexRequested(const QByteArray& array);
    void updateNormalVertexRequested(const QByteArray& array);

    void contourMaxValueChanged(const float& value);
    void contourMinValueChanged(const float& value);
};








class SurfaceMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    SurfaceMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
    void setData(const QByteArray& data, const unsigned int& rows, const unsigned int& cols)
    { static_cast<SurfaceGeometry*>(geometry())->setData(data, rows, cols); }

    void setDefaultSurface()
    { static_cast<SurfaceGeometry*>(geometry())->setDefaultSurface(); }
};






#endif // SURFACEMESH_H
