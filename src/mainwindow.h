#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scenemanager.h"
#include "componentmanager.h"

class Window3D;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupLayout();

private:
    Window3D *window3d;
    QWidget *widget3d;
    QDockWidget *componentDockWidget;
    QDockWidget *sceneDockWidget;
    SceneManager *sceneManager;
    ComponentManager *componentManager;

signals:

};

#endif // MAINWINDOW_H
