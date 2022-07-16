#include "3dwindow.h"

#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QPhongMaterial>


#include <QStandardItemModel>
#include <QTreeWidget>

//DEBUG
#include <Qt3DExtras/QFirstPersonCameraController>
#include "extrasentity/cameracontroller.h"

Window3D::Window3D()
    : Qt3DExtras::Qt3DWindow()
    , rootEntity(new Qt3DCore::QEntity)
    , orbitCamController(new Qt3DExtras::QOrbitCameraController(rootEntity))
{
    setRootEntity(rootEntity);

    camera()->setParent(rootEntity);
    camera()->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera()->setViewCenter(QVector3D(0, 0, 0));
    camera()->setPosition(QVector3D(0, 0, 40.0f));

    //orbitCamController->setLinearSpeed(50.0f);
    //orbitCamController->setLookSpeed(180.0f);
    //orbitCamController->setCamera(camera());

    //DEBUG
    //Qt3DExtras::QFirstPersonCameraController *c = new Qt3DExtras::QFirstPersonCameraController(camera());
    //c->setCamera(camera());

    //DEBUG
    FpsCameraController *fc = new FpsCameraController(camera());
    fc->setCamera(camera());

    //DEBUG
    registerAspect(new CustomAspect(this));  //Aspectの登録

    FpsMonitor *fps = new FpsMonitor(rootEntity);
    rootEntity->addComponent(fps);
    fps->setRollingMeanFrameCount(100);
}

CustomAspect::CustomAspect(QObject *parent)
    : Qt3DCore::QAbstractAspect(parent)
{
    auto mapper = QSharedPointer<FpsMonitorMapper>::create(this);
    registerBackendType<FpsMonitor>(mapper);
}
