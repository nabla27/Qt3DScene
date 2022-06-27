#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QOrbitCameraController>


class Window3D : public Qt3DExtras::Qt3DWindow
{
public:
    Window3D();

public slots:
    void addEntity(Qt3DCore::QEntity *entity) { entity->setParent(rootEntity); }

private:
    Qt3DCore::QEntity *rootEntity;
    Qt3DExtras::QOrbitCameraController *orbitCamController;
};

#endif // WINDOW3D_H
