#ifndef CUSTOMMESH_H
#define CUSTOMMESH_H
#include <QObject>
#include <QColor>

class GridIndices : public QObject
{
    Q_OBJECT
public:
    explicit GridIndices(QObject *parent) : QObject(parent) {}

public slots:
    void setSize(const unsigned int& rowCount, const unsigned int& colCount);
    void setColCount(const unsigned int& colCount) { this->colCount = colCount; }

    void createRectangleIndices();
    void createSingleLineIndices();
    void createTriangleIndices();

private:
    unsigned int rowCount;
    unsigned int colCount;

signals:
    void indicesCreated(const QByteArray& indices, const unsigned int& count);
};




class GridColorVertex : public QObject
{
    Q_OBJECT
public:
    explicit GridColorVertex(QObject *parent) : QObject(parent) {}

public slots:
    void createColorVertex(const QByteArray& array);
    void setSize(const unsigned int& rowCount, const unsigned int& colCount)
    { this->rowCount = rowCount; this->colCount = colCount; }

private:
    unsigned int rowCount = 0;
    unsigned int colCount = 0;
    float startValue = 0;
    float endValue = 0;
    QColor startColor;
    QColor endColor;

signals:
    void colorVertexCreated(const QByteArray& array, const unsigned int& count);
};






class GridNormalVertex : public QObject
{
    Q_OBJECT
public:
    explicit GridNormalVertex(QObject *parent) : QObject(parent) {}

public slots:
    void setSize(const unsigned int& rowCount, const unsigned int& colCount)
    { this->rowCount = rowCount; this->colCount = colCount; }

    void createNormalVertex(const QByteArray& array);

private:
    unsigned int rowCount = 0;
    unsigned int colCount = 0;

signals:
    void normalVertexCreated(const QByteArray& array, const unsigned int& count);
};





#endif // CUSTOMMESH_H
