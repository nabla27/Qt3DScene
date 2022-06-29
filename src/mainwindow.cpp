#include "mainwindow.h"
#include "3dwindow.h"
#include "utility.h"
#include <QDockWidget>
#include <QScreen>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , window3d(new Window3D)
    , widget3d(createWindowContainer(window3d))
    , componentDockWidget(new QDockWidget("Component", this))
    , sceneDockWidget(new QDockWidget("Scene", this))
    , sceneManager(new SceneManager(sceneDockWidget))
    , componentManager(new ComponentManager(componentDockWidget))
{
    setGeometry(getRectFromScreenRatio(this->screen()->size(), 0.6f, 0.6f));

    setupLayout();

    connect(sceneManager, &SceneManager::topLevelEntityCreated, window3d, &Window3D::addEntity);
    connect(sceneManager, &SceneManager::entityItemSelected, componentManager, &ComponentManager::setCurrentEntityItem);
}

void MainWindow::setupLayout()
{
    setCentralWidget(widget3d);

    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, componentDockWidget);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, sceneDockWidget);

    componentDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    componentDockWidget->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
    sceneDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    sceneDockWidget->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);

    sceneDockWidget->setWidget(sceneManager);
    componentDockWidget->setWidget(componentManager);

    sceneDockWidget->setContentsMargins(0, 0, 0, 0);
    componentDockWidget->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);
}
