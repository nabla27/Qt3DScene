#ifndef VECTOR3DMESH_H
#define VECTOR3DMESH_H

#include <Qt3DCore/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <QThread>
#include "../abstractcomponentssettingwidget.h"

class QVBoxLayout;



class Vector3DVertex : public QObject
{
    Q_OBJECT
public:
    explicit Vector3DVertex(QObject *parent) : QObject(parent) {}

public slots:
    void createVertex(const QByteArray& vec);
    void updateVertex();
    void createIndices();
    void setRange(const unsigned int& range);
    void setDelta(const float& dx);
    void setFactor(const float& cof);
    void setGrid(const unsigned int& range, const float& dx);

signals:
    void vertexCreated(const QByteArray& vec, const unsigned int& count);
    void indicesCreated(const QByteArray& indices, const unsigned int& count);

private:
    QByteArray vector;
    QByteArray vertices;
    QByteArray indices;
    unsigned int _range;
    float _dx;
    float _factor;
};









class Vector3DGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    Vector3DGeometry(Qt3DCore::QNode *parent = nullptr);
    ~Vector3DGeometry();

public:
    void setDefaultVector();
    void setData(const QByteArray& vec, const unsigned int& range);
    void setDelta(const float& dx);
    void setFactor(const float& factor);

    float delta() const { return _dx; }
    float factor() const { return _factor; }

private slots:
    void updatePosition(const QByteArray& pos, const unsigned int& count);
    void updateIndices(const QByteArray& indices, const unsigned int& count);

private:
    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QThread thread;
    Vector3DVertex *vectorVertex;

    unsigned int _range;
    float _dx;
    float _factor;

signals:
    void updateIndicesRequested();
    void rangeChanged(const unsigned int& range);
    void deltaChanged(const float& dx);
    void factorChanged(const float& factor);
    void gridChanged(const unsigned int& range, const float& dx);
    void createVectorRequested(const QByteArray& vec);
    void updateVectorRequested();
};









class Vector3DMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    Vector3DMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
    void setDefaultVector()
    { static_cast<Vector3DGeometry*>(geometry())->setDefaultVector(); }

    void setData(const QByteArray& data, const unsigned int& range)
    { static_cast<Vector3DGeometry*>(geometry())->setData(data, range); }

    void setDelta(const float& delta)
    { static_cast<Vector3DGeometry*>(geometry())->setDelta(delta); }

    void setFactor(const float& factor)
    { static_cast<Vector3DGeometry*>(geometry())->setFactor(factor); }

public:
    float delta() const
    { return static_cast<Vector3DGeometry*>(geometry())->delta(); }

    float factor() const
    { return static_cast<Vector3DGeometry*>(geometry())->factor(); }
};









class Vector3DMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT

public:
    Vector3DMeshSettingWidget(Vector3DMesh *mesh, QWidget *parent);

    enum class DataType { Default, DLL, };
    Q_ENUM(DataType)

public:
    AbstractComponentsSettingWidget *const clone() const override { return new Vector3DMeshSettingWidget(mesh, nullptr); }

private slots:
    void setDataSelector(const int index);

private:
    Vector3DMesh *mesh;
    QVBoxLayout *vLayout;
    QWidget *selectorSettingWidget;
};



#endif // VECTOR3DMESH_H
