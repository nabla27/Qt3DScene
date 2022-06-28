#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <QObject>
#include <Qt3DCore/QEntity>
#include <QTreeWidget>

#include "utility.h"



struct ECStruct : public QObject
{
    Q_OBJECT

public:
    static constexpr int enumStride = 100;
    static constexpr int enumOffset = 1000;
    enum class EntityType { Empty, Object2D, Object3D, Camera };

    enum class EntitySet
    {
        /* Empty */
        Empty = 1000,

        /* Object2D */
        Text2D = 1100, Line,

        /* Object3D */
        Cone = 1200, Cuboid, Cylinder, ExtrudedText, Plane, Sphere, Torus,

        /* Camera */
        FirstPersonCamera = 1300, OrbitCamera,
    };

    Q_ENUM(EntityType)
    Q_ENUM(EntitySet)
};









class EntityTreeWidget;

class EntityTreeItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeWidget *widget, const QStringList& text, int type);
    explicit EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeItem* item, const QStringList& text, int type);

public:
    Qt3DCore::QEntity* entity() { return _entity; }
    void setEditableMode();
    void setUnEditableMode();

private:
    void setup();

private:
    Qt3DCore::QEntity *_entity;
};










class EntityTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit EntityTreeWidget(QWidget *parent);

public slots:
    void receiveClickedAction(QTreeWidgetItem *item, int column);
};






#include <QMenu>
class EntityMenu : public QMenu
{
    Q_OBJECT
public:
    explicit EntityMenu(QWidget *parent);
};










class SceneManager : public QWidget
{
    Q_OBJECT
public:
    explicit SceneManager(QWidget *parent);

private slots:
    void requestTopLevelEntity();
    void requestSubEntity();
    void createNewEntity(QAction *addEntityAction);

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
    void entitySelected(Qt3DCore::QEntity *entity);
};



#endif // SCENEMANAGER_H
