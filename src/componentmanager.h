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










#include <QListWidget>
class ComponentsListWidget;

class ComponentListItem : public QObject, public QListWidgetItem
{
    Q_OBJECT
public:
    explicit ComponentListItem(Qt3DCore::QComponent *component, ComponentsListWidget *widget);

public:
    void update();

public:
    Qt3DCore::QComponent *component;
};

class ComponentsListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ComponentsListWidget(QWidget *parent);

private slots:
    void receiveChangedItem(QListWidgetItem *item);
    void emitRemoveRequest();
    void onCustomContextMenu(const QPoint& pos);

private:
    void setupContextMenu();

private:
    QMenu *contextMenu;

signals:
    void removeComponentRequested(Qt3DCore::QComponent *component);
};















#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSplitter>
#include "settingwidget/abstractcomponentssettingwidget.h"
class ComponentsSettingPage : public QWidget
{
    Q_OBJECT
public:
    ComponentsSettingPage(EntityTreeItem *entityItem);

public slots:
    void createComponent(const ECStruct::ComponentsSet c);

private slots:
    void showComponentsMenu();
    void receiveItemChanging();
    void setEntityName(const QString& name);
    void setEntityEnable(const bool checked);
    void removeComponent(Qt3DCore::QComponent *const component);
    void setCloneWidget(AbstractComponentsSettingWidget *w);
    void addClonedComponent();

private:
    void setupLayout();

    void createMaterialComponent(const ECStruct::ComponentsSet c);
    void createMeshComponent(const ECStruct::ComponentsSet c);
    void addComponent(Qt3DCore::QComponent *const component, AbstractComponentsSettingWidget *w);

private:
    EntityTreeItem *entityItem;
    static AbstractComponentsSettingWidget *cloneTarget;

    ComponentsMenu *componentsMenu;

    QAction *addComponentsAction;
    QAction *addClonedCompAction;
    QLineEdit *entityNameEdit;
    QCheckBox *entityEnableCheck;

    QSplitter *splitter;

    ComponentsListWidget *componentsListWidget;

    QScrollArea *scrollArea;
    QWidget *contentsArea;
    QVBoxLayout *contentsLayout;
    QList<AbstractComponentsSettingWidget*> contentsList;
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
