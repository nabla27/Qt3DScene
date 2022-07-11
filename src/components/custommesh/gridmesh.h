#ifndef GRIDMESH_H
#define GRIDMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include "custommesh.h"
#include <QThread>
#include <QWidget>
#include <QLibrary>
#include "../abstractcomponentssettingwidget.h"
#include "../layout.h"

class QPushButton;
class AbstractGridMeshDataSelector;
class QVBoxLayout;




class GridGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    GridGeometry(Qt3DCore::QNode *parent = nullptr);
    ~GridGeometry();

public:
    void setDefaultGrid();
    void setData(const QByteArray& pos, const unsigned int& rowCount, const unsigned int& colCount);

private slots:
    void updateIndices(const QByteArray& indices, const unsigned int& count);

private:
    Qt3DCore::QAttribute *positionAttribute;
    Qt3DCore::QAttribute *indexAttribute;
    Qt3DCore::QBuffer *positionBuffer;
    Qt3DCore::QBuffer *indexBuffer;

    QThread thread;
    GridIndices *gridIndices;

    unsigned int _rowCount = 0;
    unsigned int _colCount = 0;

signals:
    void updateIndicesRequested(const int& row, const int& col);
};







class GridMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    GridMesh(Qt3DCore::QNode *parent = nullptr);

public slots:
    void setData(const QByteArray& data, const unsigned int& rows, const unsigned int& cols)
    { static_cast<GridGeometry*>(geometry())->setData(data, rows, cols); }

    void setDefaultGrid() { static_cast<GridGeometry*>(geometry())->setDefaultGrid(); }
};








class GridMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT

public:
    GridMeshSettingWidget(GridMesh *mesh, QWidget *parent);

    enum class DataType { Default, DLL, };
    Q_ENUM(DataType)

public:
    AbstractComponentsSettingWidget *const clone() const override { return new GridMeshSettingWidget(mesh, nullptr); }

private slots:
    void setDataSelector(const int index);

private:
    GridMesh *mesh;
    QVBoxLayout *vLayout;
    QWidget *selectorSettingWidget;
};










class AbstractGridMeshDataSelector : public QObject
{
    Q_OBJECT
public:
    AbstractGridMeshDataSelector(QObject *parent) : QObject(parent) {}

public:
    virtual QWidget *const settingWidget(QWidget *parent) const = 0;

signals:
    void dataSelected(const QByteArray& data, const unsigned int& rows, const unsigned int& cols);
};






class GridMeshDataDllSelector : public AbstractGridMeshDataSelector
{
    Q_OBJECT
public:
    GridMeshDataDllSelector(QObject *parent);

    typedef void (*SizeFuncType)(unsigned int*, unsigned int*);
    typedef void (*DataFuncType)(float*);

    QWidget *const settingWidget(QWidget *parent) const override;

public slots:
    void setDllPath(const QString& path) { dllPath = path; }
    void setFuncSymbol(const QString& symbol) { funcSymbol = symbol; }
    void load();
    void unload();

private:
    QLibrary lib;

    SizeFuncType sizeFunc;
    DataFuncType dataFunc;

    QString dllPath;
    QString funcSymbol;
    static const QString sizeFuncName;
    static const QString dataFuncName;

signals:
    void dllStateChanged(const DllSelectorWidget::DllState& state);
};






#endif // GRIDMESH_H
