#include "scenemanager.h"
#include "componentmanager.h"

EntityTreeItem::EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeWidget *widget, const QStringList& name, int type)
    : QTreeWidgetItem(widget, name, type)
    , entity(entity)
    , componentsSetting(new ComponentsSettingPage(this))
{
    setup();
}

EntityTreeItem::EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeItem *item, const QStringList& name, int type)
    : QTreeWidgetItem(item, name, type)
    , entity(entity)
    , componentsSetting(new ComponentsSettingPage(this))
{
    setup();
}

void EntityTreeItem::setup()
{
    setUnEditableMode();
    setCheckState(0, Qt::CheckState::Checked);

    connect(this, &EntityTreeItem::destroyed, entity, &Qt3DCore::QEntity::deleteLater);
}

void EntityTreeItem::setEditableMode()
{
    setFlags(Qt::ItemFlag::ItemIsEnabled |
             Qt::ItemFlag::ItemIsSelectable |
             Qt::ItemFlag::ItemIsUserCheckable |
             Qt::ItemFlag::ItemIsEditable);
}

void EntityTreeItem::setUnEditableMode()
{
    setFlags(Qt::ItemFlag::ItemIsEnabled |
             Qt::ItemFlag::ItemIsSelectable |
             Qt::ItemFlag::ItemIsUserCheckable);
}









EntityTreeWidget::EntityTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setHeaderHidden(true);
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    connect(this, &EntityTreeWidget::itemClicked, this, &EntityTreeWidget::receiveClickedAction);
    connect(this, &EntityTreeWidget::itemChanged, this, &EntityTreeWidget::emitItemChanging);
}

void EntityTreeWidget::receiveClickedAction(QTreeWidgetItem *item, int column)
{
    if(column == 0)
    {
        const bool isChecked = (item->checkState(0) == Qt::CheckState::Checked) ? true : false;
        static_cast<EntityTreeItem*>(item)->entity->setEnabled(isChecked);
    }
}

void EntityTreeWidget::emitItemChanging(QTreeWidgetItem *item, int column)
{
    if(EntityTreeItem *entityItem = static_cast<EntityTreeItem*>(item))
    {
        emit entityItem->itemChanged();
    }
}








#include <QAction>
EntityMenu::EntityMenu(QWidget *parent)
    : QMenu(parent)
{
    QMenu *parentMenu = nullptr;

    const int enumCount = getEnumCount(ECStruct::EntitySet(0));
    int count = 0;

    for(int i = 0; count < enumCount; ++i)
    {
        const QString name = enumToString(ECStruct::EntitySet(i));

        if(!name.isEmpty())
        {
            if((i - ECStruct::enumOffset) % ECStruct::enumStride == 0)
                parentMenu = this->addMenu(enumToString(ECStruct::EntityType((i - ECStruct::enumOffset) / ECStruct::enumStride)));

            if(parentMenu)
            {
                auto emitSelectedItem = [this, i]() { emit selected(ECStruct::EntitySet(i)); };
                QAction *action = parentMenu->addAction(name);
                connect(action, &QAction::triggered, emitSelectedItem);
            }

            count++;
        }
    }
}














#include <QToolBar>
#include <QVBoxLayout>

#include <Qt3DExtras/QText2DEntity>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>

SceneManager::SceneManager(QWidget *parent)
    : QWidget(parent)
    , entityTree(new EntityTreeWidget(this))
    , entityMenu(new EntityMenu(this))
    , contextMenu(new QMenu(this))
    , targetEntityItem(nullptr)
{
    setupLayout();
    setupContextMenu();

    connect(entityMenu, &EntityMenu::selected, this, &SceneManager::createNewEntity);
    connect(entityTree, &EntityTreeWidget::customContextMenuRequested, this, &SceneManager::onContextMenu);
    connect(entityTree, &EntityTreeWidget::itemDoubleClicked, this, &SceneManager::emitSelectedEntity);
}

void SceneManager::setupLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QToolBar *toolBar = new QToolBar(this);

    vLayout->addWidget(toolBar);
    vLayout->addWidget(entityTree);

    toolBar->setIconSize(QSize(15, 15));
    QAction *addEntityAction = toolBar->addAction(QIcon(QPixmap(":/icon/plus")), "Add entity");

    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    toolBar->setContentsMargins(0, 0, 0, 0);
    toolBar->layout()->setSpacing(0);
    toolBar->layout()->setContentsMargins(0, 0, 0, 0);
    entityTree->setContentsMargins(0, 0, 0, 0);

    connect(addEntityAction, &QAction::triggered, this, &SceneManager::requestTopLevelEntity);
}

void SceneManager::setupContextMenu()
{
    QAction *addEntityAction = contextMenu->addMenu(entityMenu);
    QAction *renameEntityAction = contextMenu->addAction("Rename");
    QAction *removeEntityAction = contextMenu->addAction("Remove");

    addEntityAction->setText("Add sub entity");
    connect(renameEntityAction, &QAction::triggered, this, &SceneManager::renameSelectedItem);
    connect(removeEntityAction, &QAction::triggered, this, &SceneManager::removeSelectedItem);
}

void SceneManager::requestTopLevelEntity()
{
    targetEntityItem = nullptr;
    entityMenu->exec(cursor().pos());
}

void SceneManager::onContextMenu(const QPoint& pos)
{
    if(EntityTreeItem *currentItem = static_cast<EntityTreeItem*>(entityTree->currentItem())) targetEntityItem = currentItem;
    contextMenu->exec(mapToGlobal(pos));
}

void SceneManager::renameSelectedItem()
{
    if(EntityTreeItem *currentItem = static_cast<EntityTreeItem*>(entityTree->currentItem()))
    {
        currentItem->setEditableMode();
        entityTree->editItem(currentItem);
        currentItem->setUnEditableMode();
    }
}

void SceneManager::removeSelectedItem()
{
    if(QTreeWidgetItem *currentItem = entityTree->currentItem())
    {
        if(QTreeWidgetItem *parent = currentItem->parent())
            parent->removeChild(currentItem);
        else
            entityTree->takeTopLevelItem(entityTree->indexOfTopLevelItem(currentItem));

        delete currentItem;
        currentItem = nullptr;
    }
}

void SceneManager::emitSelectedEntity(QTreeWidgetItem *item, int column)
{
    if(column != 0) return;

    if(EntityTreeItem *entityItem = static_cast<EntityTreeItem*>(item))
    {
        emit entityItemSelected(entityItem);
    }
}

void SceneManager::createNewEntity(ECStruct::EntitySet entityEnum)
{
    Qt3DCore::QEntity *entity = nullptr;
    QList<ECStruct::ComponentsSet> components;

    switch(entityEnum)
    {
    case ECStruct::EntitySet::Empty:
    {
        break;
    }
    case ECStruct::EntitySet::Text2D:
    {
        Qt3DExtras::QText2DEntity *text2dEntity = new Qt3DExtras::QText2DEntity();
        text2dEntity->setText("Text 2D");
        text2dEntity->setWidth(100.0f);
        text2dEntity->setHeight(20.0f);
        text2dEntity->setColor(Qt::black);
        entity = text2dEntity;

        components << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::Line:
    {
        break;
    }
    case ECStruct::EntitySet::Cone:
    {
        components << ECStruct::ComponentsSet::ConeMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::Cuboid:
    {
        components << ECStruct::ComponentsSet::CuboidMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::Cylinder:
    {
        components << ECStruct::ComponentsSet::CylinderMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::ExtrudedText:
    {
        components << ECStruct::ComponentsSet::ExtrudedTextMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::Plane:
    {
        components << ECStruct::ComponentsSet::PlaneMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::Sphere:
    {
        components << ECStruct::ComponentsSet::SphereMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::Torus:
    {
        components << ECStruct::ComponentsSet::TorusMesh
                   << ECStruct::ComponentsSet::PhongMaterial
                   << ECStruct::ComponentsSet::Transform;
        break;
    }
    case ECStruct::EntitySet::FirstPersonCamera:
    {
        break;
    }
    case ECStruct::EntitySet::OrbitCamera:
    {
        break;
    }
    default:
        return;
    }

    if(!entity)
        entity = new Qt3DCore::QEntity;


    EntityTreeItem *entityTreeItem;
    const QString entityName = enumToString(entityEnum);
    if(targetEntityItem)
    {
        entity->setParent(targetEntityItem->entity);
        entityTreeItem = new EntityTreeItem(entity, targetEntityItem, QStringList() << entityName, (int)entityEnum);
        targetEntityItem->addChild(entityTreeItem);
    }
    else
    {
        entityTreeItem = new EntityTreeItem(entity, entityTree, QStringList() << entityName, (int)entityEnum);
        entityTree->addTopLevelItem(entityTreeItem);
        emit topLevelEntityCreated(entity);
    }

    for(const ECStruct::ComponentsSet& c : components)
        entityTreeItem->componentsSetting->createComponent(c);
}






