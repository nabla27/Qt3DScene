#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include <Qt3DCore/QEntity>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>
#include <Qt3DInput/QKeyboardDevice>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DLogic/QFrameAction>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QButtonAxisInput>
#include <Qt3DInput/QLogicalDevice>

#include <Qt3DExtras/QAbstractCameraController>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QRayCaster>

class FpsCameraController : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    explicit FpsCameraController(Qt3DCore::QNode *parent);

public:
    void setCamera(Qt3DRender::QCamera *camera);

private:
    void setup();
    void applyInputAccelerations();
    void moveCamera(const float& dt);

private:
    Qt3DRender::QCamera *camera;

    Qt3DInput::QAction *leftMouseButtonAction;
    Qt3DInput::QAction *middleMouseButtonAction;
    Qt3DInput::QAction *physicsButtonAction;

    Qt3DInput::QActionInput *leftMouseButtonInput;
    Qt3DInput::QActionInput *middleMouseButtonInput;
    Qt3DInput::QActionInput *physicsButtonInput;

    Qt3DInput::QAnalogAxisInput *mouseXInput;
    Qt3DInput::QAnalogAxisInput *mouseYInput;

    Qt3DInput::QAxis *keybordXAxis;
    Qt3DInput::QAxis *keybordZAxis;
    Qt3DInput::QAxis *mouseXAxis;
    Qt3DInput::QAxis *mouseYAxis;

    Qt3DInput::QButtonAxisInput *keybordWInput;
    Qt3DInput::QButtonAxisInput *keybordAInput;
    Qt3DInput::QButtonAxisInput *keybordSInput;
    Qt3DInput::QButtonAxisInput *keybordDInput;

    Qt3DInput::QKeyboardDevice *keybordDevice;
    Qt3DInput::QMouseDevice *mouseDevice;

    Qt3DInput::QLogicalDevice *logicalDevice;

    Qt3DLogic::QFrameAction *frameAction;

    Qt3DRender::QRayCaster *rayCaster;

    float linearSpeed;
    float lookSpeed;

    float acceleration;
    float deceleration;

    bool isPhysics = false;
    float rayDistance = 0.0f;
    QVector3D grand;

    QVector3D sceneUp;
};

#endif // CAMERACONTROLLER_H
