#ifndef SURFACEMESH_H
#define SURFACEMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include <QThread>
#include "custommesh.h"
#include "../abstractcomponentssettingwidget.h"

class QVBoxLayout;



class SurfaceGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT
public:
    SurfaceGeometry(Qt3DCore::QNode *parent = nullptr);
    ~SurfaceGeometry();

public:
    void setDefaultSurface();
    void setData(const QByteArray& pos, const unsigned int& rowCount, const unsigned int& colCount);
    void setColorMapMaxValue(const float& value) { emit colorMapMaxValueChanged(value); }
    void setColorMapMinValue(const float& value) { emit colorMapMinValueChanged(value); }
    void setColorMapType(const GridColorVertex::ColorMapType& type) { emit colorMapTypeChanged(type); }

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

    void colorMapMaxValueChanged(const float& value);
    void colorMapMinValueChanged(const float& value);
    void colorMapTypeChanged(const GridColorVertex::ColorMapType& type);
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

    void setColorMapMinValue(const float& min)
    { static_cast<SurfaceGeometry*>(geometry())->setColorMapMinValue(min); }

    void setColorMapMaxValue(const float& max)
    { static_cast<SurfaceGeometry*>(geometry())->setColorMapMaxValue(max); }

    void setColorMapType(const int& index)
    { static_cast<SurfaceGeometry*>(geometry())->setColorMapType(GridColorVertex::ColorMapType(index)); }
};









class SurfaceMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT

public:
    SurfaceMeshSettingWidget(SurfaceMesh *mesh, QWidget *parent);

public:
    AbstractComponentsSettingWidget *const clone() const override { return new SurfaceMeshSettingWidget(mesh, nullptr); }

private slots:
    void setDataSelector(const int index);

private:
    SurfaceMesh *mesh;
    QVBoxLayout *vLayout;
    QWidget *selectorSettingWidget;
};




#endif // SURFACEMESH_H
