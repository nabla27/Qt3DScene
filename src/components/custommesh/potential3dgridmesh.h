#ifndef POTENTIAL3DGRIDMESH_H
#define POTENTIAL3DGRIDMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include <QThread>

class Contour3DGridGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    explicit Contour3DGridGeometry(Qt3DCore::QNode *parent = nullptr);
    ~Contour3DGridGeometry();

public:
    void setDefaultGrid();
    void setData(const QByteArray& pos, const unsigned int& rangeX, const unsigned int& rangeY, const unsigned int& rangeZ);
    void setPotentialValue(const float& value) { this->potential = value; }
    void setThreshold(const float& threshold) { this->threshold = threshold; }

private slots:
    void updateIndices(const QByteArray& indices, const unsigned int& count);

private:
    QList<QList<float> > data;

    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QThread thread;

    float potential = 1.0f;
    float threshold = 0.2f;

    unsigned int range_x = 0;
    unsigned int range_y = 0;
    unsigned int range_z = 0;

signals:
    void updateIndicesRequested(const QByteArray& data,
                                const unsigned int& rangeX,
                                const unsigned int& rangeY,
                                const unsigned int& rangeZ,
                                const float& potential,
                                const float& threshold);
};






class Contour3DGridMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    Contour3DGridMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
};



#endif // POTENTIAL3DGRIDMESH_H
