#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <QObject>
#include <Qt3DCore/QEntity>
#include <QTreeWidget>

#include "utility.h"
#include "ecstruct.h"


class EntityTreeWidget;
class ComponentsSettingPage;

class EntityTreeItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeWidget *widget, const QStringList& name, int type);
    explicit EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeItem* item, const QStringList& name, int type);

public:
    void setEditableMode();
    void setUnEditableMode();

private:
    void setup();

public:
    Qt3DCore::QEntity *entity;
    ComponentsSettingPage *componentsSetting;

signals:
    void itemChanged();
};










class EntityTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit EntityTreeWidget(QWidget *parent);

private slots:
    void receiveClickedAction(QTreeWidgetItem *item, int column);
    void emitItemChanging(QTreeWidgetItem *item, int column);
};






#include <QMenu>
class EntityMenu : public QMenu
{
    Q_OBJECT
public:
    explicit EntityMenu(QWidget *parent);

signals:
    void selected(ECStruct::EntitySet entityEnum);
};













class SceneManager : public QWidget
{
    Q_OBJECT
public:
    explicit SceneManager(QWidget *parent);

private slots:
    void requestTopLevelEntity();
    void createNewEntity(ECStruct::EntitySet entityEnum);

    void onContextMenu(const QPoint& pos);
    void renameSelectedItem();
    void removeSelectedItem();

    void emitSelectedEntity(QTreeWidgetItem *item, int column);

private:
    void setupLayout();
    void setupContextMenu();

private:
    EntityTreeWidget *entityTree;
    EntityMenu *entityMenu;
    QMenu *contextMenu;
    EntityTreeItem *targetEntityItem;

signals:
    void topLevelEntityCreated(Qt3DCore::QEntity *entity);
    void entityItemSelected(EntityTreeItem *entityItem);
};



#endif // SCENEMANAGER_H
