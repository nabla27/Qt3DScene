#ifndef VECTOR3DMESH_H
#define VECTOR3DMESH_H

#include <Qt3DCore/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <QThread>





class Vector3DVertex : public QObject
{
    Q_OBJECT
public:
    explicit Vector3DVertex(QObject *parent) : QObject(parent) {}

public slots:
    void createVertex(const QByteArray& vec);
    void setRange(const unsigned int& range);
    void setDelta(const float& dx);
    void setGrid(const unsigned int& range, const float& dx);

signals:
    void vertexCreated(const QByteArray& vec, const unsigned int& count);
    void indicesCreated(const QByteArray& indices, const unsigned int& count);
};









class Vector3DGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    Vector3DGeometry(Qt3DCore::QNode *parent = nullptr);
    ~Vector3DGeometry();

public:
    void setDefaultVector();
    void setData(const QByteArray& vec, const unsigned int& range, const float& dx);

private slots:
    void updatePosition(const QByteArray& pos, const unsigned int& count);
    void updateIndices(const QByteArray& indices, const unsigned int& count);

private:
    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QThread thread;

    unsigned int _range;
    float _dx;

signals:
    void updateIndicesRequested();
    void rangeChanged(const unsigned int& range);
    void deltaChanged(const float& dx);
    void gridChanged(const unsigned int& range, const float& dx);
    void updateVectorRequested(const QByteArray& vec);
};









class Vector3DMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    Vector3DMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
    void setDefaultVector()
    { static_cast<Vector3DGeometry*>(geometry())->setDefaultVector(); }

    void setData(const QByteArray& data, const unsigned int& range, const float& dx)
    { static_cast<Vector3DGeometry*>(geometry())->setData(data, range, dx); }
};

#endif // VECTOR3DMESH_H
