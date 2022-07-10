#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QOrbitCameraController>


class Window3D : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    Window3D();

public slots:
    void addEntity(Qt3DCore::QEntity *entity) { entity->setParent(rootEntity); }

private:
    Qt3DCore::QEntity *rootEntity;
    Qt3DExtras::QOrbitCameraController *orbitCamController;
};



//DEBUG
#include <Qt3DCore/QAbstractAspect>
#include <Qt3DCore/QAspectJobPtr>
#include <QAspectJobPtr>
#include <vector>

#include <QBackendNode>
#include <QNode>
class FpsMonitorBackend : public Qt3DCore::QBackendNode
{
public:
    FpsMonitorBackend()
        : Qt3DCore::QBackendNode(Qt3DCore::QBackendNode::ReadWrite)
        , m_rollingMeanFrameCount(5) {}

private:
    void syncFromFrontEnd(const Qt3DCore::QNode *frontEnd, bool firstTime) override
    {
    }

private:
    int m_rollingMeanFrameCount;
};




class FpsMonitor : public Qt3DCore::QComponent
{
    Q_OBJECT

public:
    explicit FpsMonitor(Qt3DCore::QNode *parent = nullptr)
        : Qt3DCore::QComponent(parent) {}

    float fmaesPerSecond() const { return m_framesPerSecond; }
    int rollingMeanFrameCount() const { return m_rollingMeanFrameCount; }

public slots:
    void setRollingMeanFrameCount(int rollingMeanFrameCount)
    { m_rollingMeanFrameCount = rollingMeanFrameCount; }

signals:
    void framesPerSecondChanged(float framesPerSecond);
    void rollingMeanFrameCountChanged(int rollingMeanFrameCount);

private:
    float m_framesPerSecond;
    int m_rollingMeanFrameCount;
};





class FpsMonitorMapper;
class CustomAspect : public Qt3DCore::QAbstractAspect
{
    Q_OBJECT
public:
    explicit CustomAspect(QObject *parent = nullptr);

public:
    void addFpsMonitor(Qt3DCore::QNodeId id, FpsMonitorBackend *fpsMonitor)
    {
        m_fpsMonitors.insert(id, fpsMonitor);
    }

    FpsMonitorBackend *fpsMonitor(Qt3DCore::QNodeId id)
    {
        return m_fpsMonitors.value(id);
    }

    FpsMonitorBackend *takeFpsMonitor(Qt3DCore::QNodeId id)
    {
        return m_fpsMonitors.take(id);
    }

protected:
    std::vector<Qt3DCore::QAspectJobPtr> jobsToExecute(qint64 time) override
    {
        return {};
    }

private:
    QHash<Qt3DCore::QNodeId, FpsMonitorBackend*> m_fpsMonitors;
};





class FpsMonitorMapper : public Qt3DCore::QBackendNodeMapper
{
public:
    explicit FpsMonitorMapper(CustomAspect *aspect)
        : m_aspect(aspect) {}

    Qt3DCore::QBackendNode *create(const Qt3DCore::QNodeId id) const override
    {
        auto fpsMonitor = new FpsMonitorBackend;
        m_aspect->addFpsMonitor(id, fpsMonitor);
        return fpsMonitor;
    }

    Qt3DCore::QBackendNode *get(Qt3DCore::QNodeId id) const override
    {
        return m_aspect->fpsMonitor(id);
    }

    void destroy(Qt3DCore::QNodeId id) const override
    {
        auto fpsMonitor = m_aspect->takeFpsMonitor(id);
        delete fpsMonitor;
    }

private:
    CustomAspect *m_aspect;
};



#endif // WINDOW3D_H
