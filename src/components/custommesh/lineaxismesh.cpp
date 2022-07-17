#include "lineaxismesh.h"


LineAxisGeometry::LineAxisGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , colorAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , colorBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
    , xAxisColor({ 1.0f, 0.0f, 0.0f }) //red
    , yAxisColor({ 0.0f, 1.0f, 0.0f }) //green
    , zAxisColor({ 0.0f, 0.0f, 1.0f }) //blue
    , _length(10.0f)
    , _origin({ 0.0f, 0.0f, 0.0f })
{
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(positionBuffer);
    positionAttribute->setVertexSize(3); //(x,y,z)
    positionAttribute->setByteStride(3 * sizeof(float)); //(x,y,z) * float
    positionAttribute->setCount(3 * 2); //(xAxis,yAxis,zAxis) * (start,end)

    colorAttribute->setName(Qt3DCore::QAttribute::defaultColorAttributeName());
    colorAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    colorAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    colorAttribute->setBuffer(colorBuffer);
    colorAttribute->setVertexSize(3); //(r,g,b)
    colorAttribute->setByteStride(3 * sizeof(float)); //(r,g,b) * float
    colorAttribute->setCount(3 * 2); //(xAxis,yAxis,zAxis) * (start,end)

    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedShort);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(3 * 2); //(xAxis,yAxis,zAxis) * (start,end)

    addAttribute(positionAttribute);
    addAttribute(colorAttribute);
    addAttribute(indexAttribute);

    setupColorVertices();
    updatePosVertices();
    setupIndices();
}

void LineAxisGeometry::setupIndices()
{
    QByteArray indexArray;
    indexArray.resize(3 * 2 * sizeof(unsigned short)); //(xAxis,yAxis,zAxis) * (start,end) * unsigned short
    unsigned short *indices = reinterpret_cast<unsigned short*>(indexArray.data());

    for(unsigned short i = 0; i < 6; ++i)
        *indices++ = i;

    indexBuffer->setData(indexArray);
}

void LineAxisGeometry::setupColorVertices()
{
    QByteArray colorVertices;
    colorVertices.resize(3 * 2 * 3 * sizeof(float)); //(xAxis,yAxis,zAxis) * (r,g,b) * (start,end) * float
    float *c = reinterpret_cast<float*>(colorVertices.data());

    for(const QVector3D& vec : { xAxisColor, yAxisColor, zAxisColor })
    {
        //start vertex
        *c++ = vec.x();
        *c++ = vec.y();
        *c++ = vec.z();

        //end vertex
        *c++ = vec.x();
        *c++ = vec.y();
        *c++ = vec.z();
    }

    colorBuffer->setData(colorVertices);
}

void LineAxisGeometry::updatePosVertices()
{
    QVector3D axisX = _origin + QVector3D(_length, 0.0f, 0.0f);
    QVector3D axisY = _origin + QVector3D(0.0f, _length, 0.0f);
    QVector3D axisZ = _origin + QVector3D(0.0f, 0.0f, _length);

    QByteArray posVertices;
    posVertices.resize(3 * 2 * 3 * sizeof(float)); //(x,y,z) * (xAxis,yAxis,zAxis) * (start,end) * float
    float *p = reinterpret_cast<float*>(posVertices.data());

    for(const QVector3D& vec : { axisX, axisY, axisZ })
    {
        *p++ = _origin.x();
        *p++ = _origin.y();
        *p++ = _origin.z();

        *p++ = vec.x();
        *p++ = vec.y();
        *p++ = vec.z();
    }

    positionBuffer->setData(posVertices);
}

void LineAxisGeometry::setLength(const float &length)
{
    if(!qFuzzyCompare(length, _length))
    {
        _length = length;
        updatePosVertices();
        emit lengthChanged(_length);
    }
}

void LineAxisGeometry::setOrigin(const QVector3D &origin)
{
    if(origin != _origin)
    {
        _origin = origin;
        updatePosVertices();
        emit originChanged(_origin);
    }
}








LineAxisMesh::LineAxisMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new LineAxisGeometry(this));

    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}








#include <QFormLayout>
#include "../layout.h"
LineAxisMeshSettingWidget::LineAxisMeshSettingWidget(LineAxisMesh *mesh, QWidget *parent)
    : AbstractComponentsSettingWidget(mesh, "LineAxis", parent)
    , mesh(mesh)
{
    QFormLayout *fLayout = new QFormLayout(contents);
    Form3DDoubleSpin *originSpinBox = new Form3DDoubleSpin(contents);
    QDoubleSpinBox *lengthSpinBox = new QDoubleSpinBox(contents);

    contents->setLayout(fLayout);
    fLayout->addRow("Origin", originSpinBox);
    fLayout->addRow("Length", lengthSpinBox);

    constexpr float spinBoxRange = 100000.0f;
    originSpinBox->setMinimum(-spinBoxRange);
    originSpinBox->setMaximum(spinBoxRange);
    lengthSpinBox->setMaximum(spinBoxRange);

    originSpinBox->setValue(mesh->origin());
    lengthSpinBox->setValue(mesh->length());

    connect(originSpinBox, &Form3DDoubleSpin::valueChanged, mesh, &LineAxisMesh::setOrigin);
    connect(lengthSpinBox, &QDoubleSpinBox::valueChanged, mesh, &LineAxisMesh::setLength);
    connect(static_cast<LineAxisGeometry*>(mesh->geometry()), &LineAxisGeometry::originChanged, originSpinBox, &Form3DDoubleSpin::setValue);
    connect(static_cast<LineAxisGeometry*>(mesh->geometry()), &LineAxisGeometry::lengthChanged, lengthSpinBox, &QDoubleSpinBox::setValue);
}











