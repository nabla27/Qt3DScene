#ifndef LINEMESH_H
#define LINEMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include "../abstractcomponentssettingwidget.h"



class LineGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    LineGeometry(Qt3DCore::QNode *parent = nullptr);

public:
    void setDefaultLine();
    void setDirection(const QVector3D& direction);
    void setLength(const float& length);
    void setOrigin(const QVector3D& origin);
    void setLine(const QVector3D& origin, const QVector3D& direction, const float& length);

    QVector3D direction() const { return _direction; }
    QVector3D origin() const { return _origin; }
    float length() const { return _length; }

private:
    void updateLine();

private:
    QVector3D _origin;
    QVector3D _direction;
    float _length;

    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *indexBuffer;
    QByteArray positionArray;

signals:
    void originChanged(const QVector3D& origin);
    void directionChanged(const QVector3D& direction);
    void lengthChanged(const float& length);
};








class LineMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    LineMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
    void setDefaultLine()
    { static_cast<LineGeometry*>(geometry())->setDefaultLine(); }

    void setDirection(const QVector3D& direction)
    { static_cast<LineGeometry*>(geometry())->setDirection(direction); }

    void setLength(const float& length)
    { static_cast<LineGeometry*>(geometry())->setLength(length); }

    void setOrigin(const QVector3D& origin)
    { static_cast<LineGeometry*>(geometry())->setOrigin(origin); }

    void setLine(const QVector3D& origin, const QVector3D& direction, const float& length)
    { static_cast<LineGeometry*>(geometry())->setLine(origin, direction, length); }
};








class LineMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT

public:
    LineMeshSettingWidget(LineMesh *mesh, QWidget *parent);

public:
    AbstractComponentsSettingWidget *const clone() const override { return new LineMeshSettingWidget(mesh, nullptr); }

private:
    LineMesh *mesh;
};






#endif // LINEMESH_H
