#include "cameracontroller.h"

#include <Qt3DExtras/QCylinderMesh>

FpsCameraController::FpsCameraController(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , leftMouseButtonAction(new Qt3DInput::QAction(this))
    , middleMouseButtonAction(new Qt3DInput::QAction(this))
    , physicsButtonAction(new Qt3DInput::QAction(this))
    , leftMouseButtonInput(new Qt3DInput::QActionInput(this))
    , middleMouseButtonInput(new Qt3DInput::QActionInput(this))
    , physicsButtonInput(new Qt3DInput::QActionInput(this))
    , mouseXInput(new Qt3DInput::QAnalogAxisInput(this))
    , mouseYInput(new Qt3DInput::QAnalogAxisInput(this))
    , keybordXAxis(new Qt3DInput::QAxis(this))
    , keybordZAxis(new Qt3DInput::QAxis(this))
    , mouseXAxis(new Qt3DInput::QAxis(this))
    , mouseYAxis(new Qt3DInput::QAxis(this))
    , keybordWInput(new Qt3DInput::QButtonAxisInput(this))
    , keybordAInput(new Qt3DInput::QButtonAxisInput(this))
    , keybordSInput(new Qt3DInput::QButtonAxisInput(this))
    , keybordDInput(new Qt3DInput::QButtonAxisInput(this))
    , keybordDevice(new Qt3DInput::QKeyboardDevice(this))
    , mouseDevice(new Qt3DInput::QMouseDevice(this))
    , logicalDevice(new Qt3DInput::QLogicalDevice(this))
    , frameAction(new Qt3DLogic::QFrameAction(this))
    , rayCaster(new Qt3DRender::QRayCaster(this))
    , linearSpeed(10.0f)
    , lookSpeed(180.0f)
    , acceleration(-1.0f)
    , deceleration(-1.0f)
    , sceneUp(0.0f, 1.0f, 0.0f)
{
    setup();
}

void FpsCameraController::setup()
{
    /* left mouse button action */
    leftMouseButtonInput->setButtons(QList<int>{ Qt::LeftButton });
    leftMouseButtonInput->setSourceDevice(mouseDevice);
    leftMouseButtonAction->addInput(leftMouseButtonInput);

    /* middle mouse buttton action */
    middleMouseButtonInput->setButtons(QList<int>{ Qt::MiddleButton });
    middleMouseButtonInput->setSourceDevice(mouseDevice);
    middleMouseButtonAction->addInput(middleMouseButtonInput);

    physicsButtonInput->setButtons(QList<int>{ Qt::Key_E });
    physicsButtonInput->setSourceDevice(keybordDevice);
    physicsButtonAction->addInput(physicsButtonInput);
    connect(physicsButtonAction, &Qt3DInput::QAction::activeChanged, [this](const bool isActive){
        if(isActive) isPhysics = !isPhysics;
    });

    mouseXInput->setAxis(Qt3DInput::QMouseDevice::X);
    mouseXInput->setSourceDevice(mouseDevice);
    mouseXAxis->addInput(mouseXInput);

    mouseYInput->setAxis(Qt3DInput::QMouseDevice::Y);
    mouseYInput->setSourceDevice(mouseDevice);
    mouseYAxis->addInput(mouseYInput);

    /* keybord pos w */
    keybordWInput->setButtons(QList<int>{ Qt::Key_W });
    keybordWInput->setScale(1.0f);
    keybordWInput->setSourceDevice(keybordDevice);
    keybordZAxis->addInput(keybordWInput);

    /* keybord pos a */
    keybordAInput->setButtons(QList<int>{ Qt::Key_A });
    keybordAInput->setScale(-1.0f);
    keybordAInput->setSourceDevice(keybordDevice);
    keybordXAxis->addInput(keybordAInput);

    /* keybord pos s */
    keybordSInput->setButtons(QList<int>{ Qt::Key_S });
    keybordSInput->setScale(-1.0f);
    keybordSInput->setSourceDevice(keybordDevice);
    keybordZAxis->addInput(keybordSInput);

    /* keybord pos d */
    keybordDInput->setButtons(QList<int>{ Qt::Key_D });
    keybordDInput->setScale(1.0f);
    keybordDInput->setSourceDevice(keybordDevice);
    keybordXAxis->addInput(keybordDInput);

    /* logical device */
    logicalDevice->addAction(leftMouseButtonAction);
    logicalDevice->addAction(middleMouseButtonAction);
    logicalDevice->addAction(physicsButtonAction);
    logicalDevice->addAxis(mouseXAxis);
    logicalDevice->addAxis(mouseYAxis);
    logicalDevice->addAxis(keybordXAxis);
    logicalDevice->addAxis(keybordZAxis);

    connect(this, &FpsCameraController::enabledChanged, logicalDevice, &Qt3DInput::QLogicalDevice::setEnabled);
    connect(this, &FpsCameraController::enabledChanged, frameAction, &Qt3DLogic::QFrameAction::setEnabled);
    for(auto axis : { mouseXAxis, mouseYAxis, keybordXAxis, keybordZAxis })
        connect(this, &FpsCameraController::enabledChanged, axis, &Qt3DInput::QAxis::setEnabled);
    connect(frameAction, &Qt3DLogic::QFrameAction::triggered, this, &FpsCameraController::moveCamera);


    //addComponent(frameAction);
    //addComponent(logicalDevice);

    //DEBUG
    addComponent(rayCaster);
    connect(rayCaster, &Qt3DRender::QRayCaster::hitsChanged, [this](const Qt3DRender::QAbstractRayCaster::Hits& hits){
        if(hits.size() > 0)
        {
            rayDistance = hits.at(0).distance();
            grand = hits.at(0).localIntersection();
        }
        else
        {
            rayDistance = 0.0f;
        }
    });


    applyInputAccelerations();
}

void FpsCameraController::applyInputAccelerations()
{
    const auto inputs =
    {
        keybordWInput,
        keybordAInput,
        keybordSInput,
        keybordDInput
    };

    for(auto input : inputs)
    {
        input->setAcceleration(acceleration);
        input->setDeceleration(deceleration);
    }
}

void FpsCameraController::moveCamera(const float &dt)
{
    if(camera)
    {
        camera->translate(QVector3D(keybordXAxis->value() * linearSpeed,
                                    0,
                                    keybordZAxis->value() * linearSpeed) * dt);

        if(leftMouseButtonAction->isActive())
        {
            const QVector3D upVector(0.0f, 1.0f, 0.0f);

            camera->pan(mouseXAxis->value() * lookSpeed * dt, upVector);
            camera->tilt(mouseYAxis->value() * lookSpeed * dt);
        }

        //DEBUG
        rayCaster->trigger(camera->position(), QVector3D(0.0f, -1.0f, 0.0f), 5.0f);

        qDebug() << camera->position() << grand;

        //if(isPhysics)
        //{
        //    if(rayDistance < 0.1f)
        //    {
        //        camera->setPosition(camera->position() - QVector3D(0.0f, 0.1f, 0.0f));
        //    }
        //    else
        //    {
        //        camera->setPosition(grand + QVector3D(0.0f, 1.0f, 0.0f));
        //    }
        //}
    }
}

void FpsCameraController::setCamera(Qt3DRender::QCamera *camera)
{
    this->camera = camera;
}


