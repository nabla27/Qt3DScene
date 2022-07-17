#ifndef LINEAXISMESH_H
#define LINEAXISMESH_H


#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include "../abstractcomponentssettingwidget.h"

class LineAxisGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT
public:
    LineAxisGeometry(Qt3DCore::QNode *parent = nullptr);

public:
    void setLength(const float& length);
    void setOrigin(const QVector3D& origin);

    float length() const { return _length; }
    QVector3D origin() const { return _origin; }

private:
    void setupIndices();
    void setupColorVertices();
    void updatePosVertices();

private:
    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *colorAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *colorBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QVector3D xAxisColor;
    QVector3D yAxisColor;
    QVector3D zAxisColor;

    float _length;
    QVector3D _origin;

signals:
    void lengthChanged(const float& length);
    void originChanged(const QVector3D& origin);
};










class LineAxisMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    LineAxisMesh(Qt3DCore::QNode *parent = nullptr);

public:
    float length() const
    { return static_cast<LineAxisGeometry*>(geometry())->length(); }

    QVector3D origin() const
    { return static_cast<LineAxisGeometry*>(geometry())->origin(); }

public slots:
    void setLength(const float& length)
    { static_cast<LineAxisGeometry*>(geometry())->setLength(length); }

    void setOrigin(const QVector3D& origin)
    { static_cast<LineAxisGeometry*>(geometry())->setOrigin(origin); }
};









class LineAxisMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT
public:
    explicit LineAxisMeshSettingWidget(LineAxisMesh *mesh, QWidget *parent);

public:
    AbstractComponentsSettingWidget *const clone() const override { return new LineAxisMeshSettingWidget(mesh, nullptr); }

private:
    LineAxisMesh *mesh;
};










#endif // LINEAXISMESH_H
