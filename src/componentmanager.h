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













#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QScrollArea>
class ComponentsSettingWidget : public QWidget
{
    Q_OBJECT
public:
    ComponentsSettingWidget(EntityTreeItem *entityItem);

private slots:
    void showComponentsMenu();
    void receiveItemChanging();
    void setEntityName(const QString& name);
    void setEntityEnable(const bool checked);

private:
    void setupLayout();

private:
    EntityTreeItem *entityItem;

    ComponentsMenu *componentsMenu;

    QLineEdit *entityNameEdit;
    QCheckBox *entityEnableCheck;

    QScrollArea *scrollArea;
    QWidget *contentsArea;
    QVBoxLayout *contentsLayout;
};













#include <QStackedWidget>
class ComponentManager : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentManager(QWidget *parent = nullptr);

public slots:
    void setCurrentEntityItem(EntityTreeItem *item);

private:
    void setupLayout();

private:
    EntityTreeItem *currentEntityItem;
    QStackedWidget *settingPageStack;

signals:

};














#endif // COMPONENTMANAGER_H
