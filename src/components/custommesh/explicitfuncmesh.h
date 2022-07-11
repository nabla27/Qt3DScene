#ifndef EXPLICITFUNCMESH_H
#define EXPLICITFUNCMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QGeometry>
#include <QThread>
#include <QLibrary>
#include "custommesh.h"
#include "../abstractcomponentssettingwidget.h"
#include "../layout.h"

class QVBoxLayout;


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









class ExplicitFuncMeshSettingWidget : public AbstractComponentsSettingWidget
{
    Q_OBJECT

public:
    explicit ExplicitFuncMeshSettingWidget(ExplicitFuncMesh *mesh, QWidget *parent);
    ~ExplicitFuncMeshSettingWidget();

    enum class DataType { Default, DLL, Csv, Tsv, MathExp };
    Q_ENUM(DataType)

public:
    AbstractComponentsSettingWidget *const clone() const override
    { return nullptr; }

public slots:
    void setDataSelector(const int index);

private:
    ExplicitFuncMesh *mesh;
    QVBoxLayout *vLayout;
    QWidget *selectorSettingWidget;

    QThread thread;
};










class AbstractExplicitFuncMeshDataSelector : public QObject
{
    Q_OBJECT
public:
    explicit AbstractExplicitFuncMeshDataSelector(QObject *parent)
        : QObject(parent) {}

public:
    virtual QWidget *const settingWidget(QWidget *parent) const = 0;

signals:
    void dataSelected(const QByteArray& data, const unsigned int& samples);
};











class ExplicitFuncMeshDataDllSelector : public AbstractExplicitFuncMeshDataSelector
{
    Q_OBJECT
public:
    explicit ExplicitFuncMeshDataDllSelector(QObject *parent);

    typedef void (*SamplesFuncType)(unsigned int*);
    typedef void (*DataFuncType)(float*);

    QWidget *const settingWidget(QWidget *parent) const override;

public slots:
    void setDllPath(const QString& path) { dllPath = path; }
    void setFuncSymbol(const QString& symbol) { funcSymbol = symbol; }
    void load();
    void unload();

private:
    QLibrary lib;

    SamplesFuncType samplesFunc;
    DataFuncType dataFunc;

    QString dllPath;
    QString funcSymbol;
    static const QString samplesFuncName;
    static const QString dataFuncName;

    QByteArray array;
    unsigned int prevSamples = 0;

signals:
    void dllStateChanged(const DllSelectorWidget::DllState& state);
    void messagePushed(const QString& message);
};






#endif // EXPLICITFUNCMESH_H
