#include "scenemanager.h"

EntityTreeItem::EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeWidget *widget, const QStringList& text, int type)
    : QTreeWidgetItem(widget, text, type)
    , _entity(entity)
{
    setup();
}

EntityTreeItem::EntityTreeItem(Qt3DCore::QEntity *entity, EntityTreeItem *item, const QStringList& text, int type)
    : QTreeWidgetItem(item, text, type)
    , _entity(entity)
{
    setup();
}

void EntityTreeItem::setup()
{
    setUnEditableMode();
    setCheckState(0, Qt::CheckState::Checked);

    connect(this, &EntityTreeItem::destroyed, _entity, &Qt3DCore::QEntity::deleteLater);
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
}

void EntityTreeWidget::receiveClickedAction(QTreeWidgetItem *item, int column)
{
    if(column == 0)
    {
        const bool isChecked = (item->checkState(0) == Qt::CheckState::Checked) ? true : false;
        static_cast<EntityTreeItem*>(item)->entity()->setEnabled(isChecked);
    }
}








#include <QAction>
EntityMenu::EntityMenu(QWidget *parent)
    : QMenu(parent)
{
    QMenu *parentMenu = nullptr;

    const QStringList entitySet = enumToStrings(ECStruct::EntitySet(0));

    for(const QString& name : entitySet)
    {
        const int index = getEnumIndex<ECStruct::EntitySet>(name);
        if(index % ECStruct::enumStride == 0)
        {
            const QString title = enumToString(ECStruct::EntityType(index / ECStruct::enumStride));
            parentMenu = addMenu(title);
            parentMenu->addAction(name);
        }
        else
        {
            if(parentMenu)
            {
                parentMenu->addAction(name);
            }
        }
    }
}











#include <QToolBar>
#include <QVBoxLayout>
#include <QApplication>
#include <Qt3DExtras>

SceneManager::SceneManager(QWidget *parent)
    : QWidget(parent)
    , entityTree(new EntityTreeWidget(this))
    , entityMenu(new EntityMenu(this))
    , contextMenu(new QMenu(this))
    , targetEntityItem(nullptr)
{
    setupLayout();
    setupContextMenu();

    connect(entityMenu, &QMenu::triggered, this, &SceneManager::createNewEntity);
    connect(entityTree, &EntityTreeWidget::customContextMenuRequested, this, &SceneManager::onContextMenu);
}

void SceneManager::setupLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QToolBar *toolBar = new QToolBar(this);

    vLayout->addWidget(toolBar);
    vLayout->addWidget(entityTree);

    toolBar->setIconSize(QSize(15, 15));
    QAction *addEntityAction = toolBar->addAction(QIcon(QPixmap(":/icon/plus")), "add entity");

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
    QAction *addEntityAction = contextMenu->addAction("Add sub entity");
    QAction *renameEntityAction = contextMenu->addAction("Rename");
    QAction *removeEntityAction = contextMenu->addAction("Remove");

    connect(addEntityAction, &QAction::triggered, this, &SceneManager::requestSubEntity);
    connect(renameEntityAction, &QAction::triggered, this, &SceneManager::renameSelectedItem);
    connect(removeEntityAction, &QAction::triggered, this, &SceneManager::removeSelectedItem);
}

void SceneManager::requestTopLevelEntity()
{
    targetEntityItem = nullptr;
    entityMenu->exec(cursor().pos());
}

void SceneManager::requestSubEntity()
{
    if(entityTree->selectedItems().count() > 0)
        targetEntityItem = static_cast<EntityTreeItem*>(entityTree->selectedItems().at(0));
    else
        targetEntityItem = nullptr;

    entityMenu->exec(cursor().pos());
}

void SceneManager::onContextMenu(const QPoint& pos)
{
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

void SceneManager::createNewEntity(QAction *addEntityAction)
{
    Qt3DCore::QEntity *entity = nullptr;

    const QString entityName = addEntityAction->text();
    const ECStruct::EntitySet entityEnum = ECStruct::EntitySet(getEnumIndex<ECStruct::EntitySet>(entityName));

    switch(entityEnum)
    {
    case ECStruct::EntitySet::Empty:
    {
        break;
    }

    case ECStruct::EntitySet::Text2D:
    {
        Qt3DExtras::QText2DEntity *text2dEntity = new Qt3DExtras::QText2DEntity();
        text2dEntity->addComponent(new Qt3DCore::QTransform(entity));
        text2dEntity->setText("Text 2D");
        text2dEntity->setWidth(100.0f);
        text2dEntity->setHeight(20.0f);
        text2dEntity->setColor(Qt::black);
        entity = text2dEntity;
        break;
    }
    case ECStruct::EntitySet::Line:
    {
        break;
    }

    case ECStruct::EntitySet::Cone:
    {
        entity = new Qt3DCore::QEntity;
        entity->addComponent(new Qt3DExtras::QConeMesh(entity));
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
        break;
    }
    case ECStruct::EntitySet::Cuboid:
    {
        entity = new Qt3DCore::QEntity;
        entity->addComponent(new Qt3DExtras::QCuboidMesh(entity));
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
        break;
    }
    case ECStruct::EntitySet::Cylinder:
    {
        entity = new Qt3DCore::QEntity;
        entity->addComponent(new Qt3DExtras::QCylinderMesh(entity));
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
        break;
    }
    case ECStruct::EntitySet::ExtrudedText:
    {
        entity = new Qt3DCore::QEntity;
        Qt3DExtras::QExtrudedTextMesh *mesh = new Qt3DExtras::QExtrudedTextMesh(entity);
        entity->addComponent(mesh);
        mesh->setText("Extruded Text");
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
        break;
    }
    case ECStruct::EntitySet::Plane:
    {
        entity = new Qt3DCore::QEntity;
        entity->addComponent(new Qt3DExtras::QPlaneMesh(entity));
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
        break;
    }
    case ECStruct::EntitySet::Sphere:
    {
        entity = new Qt3DCore::QEntity;
        entity->addComponent(new Qt3DExtras::QSphereMesh(entity));
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
        break;
    }
    case ECStruct::EntitySet::Torus:
    {
        entity = new Qt3DCore::QEntity;
        entity->addComponent(new Qt3DExtras::QTorusMesh(entity));
        entity->addComponent(new Qt3DExtras::QPhongMaterial(entity));
        entity->addComponent(new Qt3DCore::QTransform(entity));
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
        break;
    }

    if(!entity) return;


    EntityTreeItem *entityTreeItem;
    if(targetEntityItem)
    {
        entity->setParent(targetEntityItem->entity());
        entityTreeItem = new EntityTreeItem(entity, targetEntityItem, QStringList() << entityName, (int)entityEnum);
        targetEntityItem->addChild(entityTreeItem);
    }
    else
    {
        entityTreeItem = new EntityTreeItem(entity, entityTree, QStringList() << entityName, (int)entityEnum);
        entityTree->addTopLevelItem(entityTreeItem);
        emit topLevelEntityCreated(entity);
    }
}






