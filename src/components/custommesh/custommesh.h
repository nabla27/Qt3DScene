#ifndef CUSTOMMESH_H
#define CUSTOMMESH_H
#include <QObject>

class GridIndices : public QObject
{
    Q_OBJECT
public:
    GridIndices(QObject *parent) : QObject(parent) {}

public slots:
    void createSquareIndices(const unsigned int& row, const unsigned int& col);
    void createSingleLineIndices(const unsigned int& pointCount);


signals:
    void created(const QByteArray& array, const unsigned int& count);
};


#endif // CUSTOMMESH_H
