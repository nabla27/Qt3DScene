#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H
#include "scenemanager.h"








class ComponentsMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ComponentsMenu(QWidget *parent);

signals:
    void selected(ECStruct::ComponentsSet componentEnum);
};












#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
class ComponentManager : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentManager(QWidget *parent = nullptr);

public slots:
    void setCurrentEntityItem(EntityTreeItem *item);

private slots:
    void showComponentsMenu();

private:
    void setupLayout();

private:
    EntityTreeItem *currentEntityItem;
    ComponentsMenu *componentsMene;

signals:

};

#endif // COMPONENTMANAGER_H
