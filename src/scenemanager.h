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

    enum class ComponentType { Transform, Material, Mesh };
    enum class ComponentsSet
    {
        /* Transform */
        Transform = 1000,

        /* Material */
        DiffuseMapMaterial = 1100, DiffuseSpecularMapMaterial, DiffuseSpecularMaterial, GoochMaterial, MetalRougthMaterial, MorphPhongMaterial,
        NormalDiffuseMapMaterial, NormalDiffuseSpecularMapMaterial, PerVertexColorMaterial, PhongAlphaMaterial, PhongMaterial, TextureMaterial,

        /* Mesh */
        Cone = 1200, Cuboid, Cylinder, ExtrudedText, Plane, Sphere, Torus,
    };


    Q_ENUM(EntityType)
    Q_ENUM(EntitySet)
    Q_ENUM(ComponentType)
    Q_ENUM(ComponentsSet)
};









class EntityTreeWidget;
class ComponentsSettingWidget;

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
    ComponentsSettingWidget *componentsSetting;

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
