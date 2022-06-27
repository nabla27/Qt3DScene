#include "3dwindow.h"

#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QPhongMaterial>


#include <QStandardItemModel>
#include <QTreeWidget>

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

    orbitCamController->setLinearSpeed(50.0f);
    orbitCamController->setLookSpeed(180.0f);
    orbitCamController->setCamera(camera());

    //Qt3DCore::QEntity *coneEntity = new Qt3DCore::QEntity(rootEntity);
    //Qt3DExtras::QConeMesh *coneMesh = new Qt3DExtras::QConeMesh(coneEntity);
    //Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial(coneEntity);
    //Qt3DCore::QTransform *transform = new Qt3DCore::QTransform(coneEntity);

    //coneEntity->addComponent(coneMesh);
    //coneEntity->addComponent(material);
    //coneEntity->addComponent(transform);




}
