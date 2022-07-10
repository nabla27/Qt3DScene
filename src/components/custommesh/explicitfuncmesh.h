#ifndef EXPLICITFUNCMESH_H
#define EXPLICITFUNCMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include <QThread>
#include "custommesh.h"


class ExplicitFuncGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    ExplicitFuncGeometry(Qt3DCore::QNode *parent = nullptr);
    ~ExplicitFuncGeometry();

public:
    void setDefaultFunc();
    void setData(const QByteArray& pos, const unsigned int& samples);

private slots:
    void updateIndices(const QByteArray& indices, const unsigned int& indexCount);

private:
    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QThread thread;
    GridIndices *gridIndices;

    unsigned int samples;

signals:
    void updateIndicesRequested(const unsigned int& samples);
};









class ExplicitFuncMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    ExplicitFuncMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
    void setData(const QByteArray& data, const unsigned int& samples)
    { static_cast<ExplicitFuncGeometry*>(geometry())->setData(data, samples); }

    void setDefaultFunc()
    { static_cast<ExplicitFuncGeometry*>(geometry())->setDefaultFunc(); }
};







#endif // EXPLICITFUNCMESH_H
