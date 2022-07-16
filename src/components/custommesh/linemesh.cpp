#include "linemesh.h"

LineGeometry::LineGeometry(Qt3DCore::QNode *parent)
    : Qt3DCore::QGeometry(parent)
    , positionAttribute(new Qt3DCore::QAttribute(this))
    , indexAttribute(new Qt3DCore::QAttribute(this))
    , positionBuffer(new Qt3DCore::QBuffer(this))
    , indexBuffer(new Qt3DCore::QBuffer(this))
{
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(positionBuffer);
    positionAttribute->setVertexSize(3); //(x,y,z)
    positionAttribute->setByteStride(3 * sizeof(float)); //(x,y,z) * float
    positionAttribute->setCount(2); //start to end

    positionArray.resize(3 * 2 * sizeof(float)); //(x,y,z) * (start,end) * float

    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedShort);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2); //start to end

    QByteArray indexArray;
    indexArray.resize(2 * sizeof(unsigned short)); //(x,y,z) * (start,end) * unsigned short
    unsigned short *i = reinterpret_cast<unsigned short*>(indexArray.data());
    *i++ = 0;
    *i++ = 1;
    indexBuffer->setData(indexArray);

    addAttribute(positionAttribute);
    addAttribute(indexAttribute);

    setDefaultLine();
}

void LineGeometry::setDefaultLine()
{
    _origin = QVector3D(0.0f, 0.0f, 0.0f);
    _direction = QVector3D(1.0f, 0.0f, 0.0f);
    _length = 10.0f;

    emit originChanged(_origin);
    emit directionChanged(_direction);
    emit lengthChanged(_length);

    updateLine();
}

void LineGeometry::setDirection(const QVector3D &direction)
{
    if(direction != _direction)
    {
        this->_direction = direction;
        updateLine();
        emit directionChanged(_direction);
    }
}

void LineGeometry::setLength(const float &length)
{
    if(!qFuzzyCompare(length, _length))
    {
        this->_length = length;
        updateLine();
        emit lengthChanged(_length);
    }
}

void LineGeometry::setOrigin(const QVector3D &origin)
{
    if(origin != _origin)
    {
        this->_origin = origin;
        updateLine();
        emit originChanged(_origin);
    }
}

void LineGeometry::setLine(const QVector3D &origin, const QVector3D &direction, const float &length)
{
    if(origin != _origin)
    {
        this->_origin = origin;
        emit originChanged(_origin);
    }
    if(_direction != direction)
    {
        this->_direction = direction;
        emit directionChanged(_direction);
    }
    if(!qFuzzyCompare(length, _length))
    {
        this->_length = length;
        emit lengthChanged(length);
    }
    updateLine();
}

void LineGeometry::updateLine()
{
    const QVector3D end = _origin + _length * _direction;

    float *p = reinterpret_cast<float*>(positionArray.data());

    *p++ = _origin.x();
    *p++ = _origin.y();
    *p++ = _origin.z();

    *p++ = end.x();
    *p++ = end.y();
    *p++ = end.z();

    positionBuffer->setData(positionArray);
}








LineMesh::LineMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setGeometry(new LineGeometry(this));

    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}









#include "../layout.h"
#include <QFormLayout>
LineMeshSettingWidget::LineMeshSettingWidget(LineMesh *mesh, QWidget *parent)
    : AbstractComponentsSettingWidget(mesh, "LineMesh", parent)
    , mesh(mesh)
{
    QFormLayout *fLayout = new QFormLayout(contents);
    Form3DDoubleSpin *originDoubleSpin = new Form3DDoubleSpin(contents);
    Form3DDoubleSpin *directionDoubleSpin = new Form3DDoubleSpin(contents);
    QDoubleSpinBox *lengthDoubleSpin = new QDoubleSpinBox(contents);

    contents->setLayout(fLayout);
    fLayout->addRow("Origin", originDoubleSpin);
    fLayout->addRow("Direction", directionDoubleSpin);
    fLayout->addRow("Length", lengthDoubleSpin);

    constexpr double spinBoxRange = 10000;
    originDoubleSpin->setMinimum(-spinBoxRange);
    originDoubleSpin->setMaximum(spinBoxRange);
    directionDoubleSpin->setMaximum(1.0);
    lengthDoubleSpin->setMinimum(-spinBoxRange);
    lengthDoubleSpin->setMaximum(spinBoxRange);

    originDoubleSpin->setValue(static_cast<LineGeometry*>(mesh->geometry())->origin());
    directionDoubleSpin->setValue(static_cast<LineGeometry*>(mesh->geometry())->direction());
    lengthDoubleSpin->setValue(static_cast<LineGeometry*>(mesh->geometry())->length());

    connect(originDoubleSpin, &Form3DDoubleSpin::valueChanged, mesh, &LineMesh::setOrigin);
    connect(directionDoubleSpin, &Form3DDoubleSpin::valueChanged, mesh, &LineMesh::setDirection);
    connect(lengthDoubleSpin, &QDoubleSpinBox::valueChanged, mesh, &LineMesh::setLength);
    connect(static_cast<LineGeometry*>(mesh->geometry()), &LineGeometry::originChanged, originDoubleSpin, &Form3DDoubleSpin::setValue);
    connect(static_cast<LineGeometry*>(mesh->geometry()), &LineGeometry::directionChanged, directionDoubleSpin, &Form3DDoubleSpin::setValue);
    connect(static_cast<LineGeometry*>(mesh->geometry()), &LineGeometry::lengthChanged, lengthDoubleSpin, &QDoubleSpinBox::setValue);
}


