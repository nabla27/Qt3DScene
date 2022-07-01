#include "componentmanager.h"







ComponentsMenu::ComponentsMenu(QWidget *parent)
    : QMenu(parent)
{
    QMenu *parentMenu = nullptr;

    const int enumCount = getEnumCount(ECStruct::ComponentsSet(0));
    int count = 0;

    for(int i = 0; count < enumCount; ++i)
    {
        const QString name = enumToString(ECStruct::ComponentsSet(i));

        if(!name.isEmpty())
        {
            if((i - ECStruct::enumOffset) % ECStruct::enumStride == 0)
                parentMenu = this->addMenu(enumToString(ECStruct::ComponentType((i - ECStruct::enumOffset) / ECStruct::enumStride)));

            if(parentMenu)
            {
                auto emitSelectedItem = [this, i]() { emit selected(ECStruct::ComponentsSet(i)); };
                QAction *action = parentMenu->addAction(name);
                connect(action, &QAction::triggered, emitSelectedItem);
            }

            count++;
        }
    }
}













#include <Qt3DCore/QComponent>
ComponentListItem::ComponentListItem(Qt3DCore::QComponent *component, const QString& name, ComponentsListWidget *widget)
    : QListWidgetItem(name, widget)
    , component(component)
{
    setCheckState(Qt::CheckState::Checked);
}

void ComponentListItem::update()
{
    if(checkState() == Qt::CheckState::Checked)
        component->setEnabled(true);
    else
        component->setEnabled(false);
}

ComponentsListWidget::ComponentsListWidget(QWidget *parent)
    : QListWidget(parent)
    , contextMenu(new QMenu(this))
{
    setupContextMenu();
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    connect(this, &ComponentsListWidget::customContextMenuRequested, this, &ComponentsListWidget::onCustomContextMenu);
    connect(this, &ComponentsListWidget::itemChanged, this, &ComponentsListWidget::receiveChangedItem);
}

void ComponentsListWidget::addComponentToList(Qt3DCore::QComponent *component, const ECStruct::ComponentsSet c)
{
    addItem(new ComponentListItem(component, enumToString(c), this));
}

void ComponentsListWidget::receiveChangedItem(QListWidgetItem *item)
{
    static_cast<ComponentListItem*>(item)->update();
}

void ComponentsListWidget::emitRemoveRequest()
{
    ComponentListItem *item = static_cast<ComponentListItem*>(currentItem());
    emit removeComponentRequested(static_cast<ComponentListItem*>(currentItem())->component);
    delete takeItem(indexFromItem(item).row());
}

void ComponentsListWidget::onCustomContextMenu(const QPoint& pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void ComponentsListWidget::setupContextMenu()
{
    QAction *requestRemove = contextMenu->addAction("Remove");

    connect(requestRemove, &QAction::triggered, this, &ComponentsListWidget::emitRemoveRequest);
}

















#include <QToolBar>
ComponentsSettingWidget::ComponentsSettingWidget(EntityTreeItem *entityItem)
    : QWidget(nullptr)
    , entityItem(entityItem)
    , componentsMenu(new ComponentsMenu(this))
    , entityNameEdit(new QLineEdit(this))
    , entityEnableCheck(new QCheckBox(this))
    , splitter(new QSplitter(Qt::Orientation::Vertical, this))
    , componentsListWidget(new ComponentsListWidget(splitter))
    , scrollArea(new QScrollArea(splitter))
    , contentsArea(new QWidget(scrollArea))
    , contentsLayout(new QVBoxLayout(contentsArea))
{
    setupLayout();

    connect(componentsMenu, &ComponentsMenu::selected, this, &ComponentsSettingWidget::createComponent);
    connect(entityItem, &EntityTreeItem::itemChanged, this, &ComponentsSettingWidget::receiveItemChanging);
    connect(entityNameEdit, &QLineEdit::textChanged, this, &ComponentsSettingWidget::setEntityName);
    connect(entityEnableCheck, &QCheckBox::toggled, this, &ComponentsSettingWidget::setEntityEnable);
    connect(componentsListWidget, &ComponentsListWidget::removeComponentRequested, this, &ComponentsSettingWidget::removeComponent);
}

void ComponentsSettingWidget::setupLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QToolBar *toolBar = new QToolBar(this);

    setLayout(vLayout);
    vLayout->addWidget(toolBar);
    QAction *addComponentsAction = toolBar->addAction(QIcon(QPixmap(":/icon/plus")), "Add component");
    toolBar->addWidget(entityNameEdit);
    toolBar->addWidget(entityEnableCheck);
    vLayout->addWidget(splitter);
    splitter->addWidget(componentsListWidget);
    splitter->addWidget(scrollArea);
    scrollArea->setWidget(contentsArea);
    contentsArea->setLayout(contentsLayout);

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    toolBar->setIconSize(QSize(15, 15));
    entityNameEdit->setFixedHeight(20);
    scrollArea->setWidgetResizable(true);
    contentsLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    toolBar->setContentsMargins(0, 0, 0, 0);
    toolBar->layout()->setSpacing(5);
    toolBar->layout()->setContentsMargins(0, 0, 5, 0);
    scrollArea->setContentsMargins(0, 0, 0, 0);
    contentsArea->setContentsMargins(0, 0, 0, 0);
    contentsLayout->setSpacing(0);
    contentsLayout->setContentsMargins(0, 0, 0, 0);

    connect(addComponentsAction, &QAction::triggered, this, &ComponentsSettingWidget::showComponentsMenu);
}

void ComponentsSettingWidget::showComponentsMenu()
{
    componentsMenu->exec(cursor().pos());
}

void ComponentsSettingWidget::receiveItemChanging()
{
    entityEnableCheck->setCheckState(entityItem->checkState(0));
    entityNameEdit->setText(entityItem->text(0));
}

void ComponentsSettingWidget::setEntityName(const QString& name)
{
    entityItem->setText(0, name);
}

void ComponentsSettingWidget::setEntityEnable(const bool checked)
{
    entityItem->entity->setEnabled(checked);
    entityItem->setCheckState(0, (checked) ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

void ComponentsSettingWidget::removeComponent(Qt3DCore::QComponent *component)
{
    entityItem->entity->removeComponent(component);
    if(component->entities().count() == 0) delete component;  //複数のEntity間で共有されている場合がある。

    for(AbstractComponentsSettingWidget *w : contentsList)
    {
        if(w->component() == component)
        {
            contentsLayout->removeWidget(w);
            delete w;
        }
    }
}

void ComponentsSettingWidget::addComponent(Qt3DCore::QComponent *component, const ECStruct::ComponentsSet c, AbstractComponentsSettingWidget *w)
{
    entityItem->entity->addComponent(component);
    componentsListWidget->addComponentToList(component, c);
    if(w)
    {
        contentsList.append(w);
        contentsLayout->addWidget(w);
    }
}

#include "settingwidget/transformwidget.h"
void ComponentsSettingWidget::createComponent(const ECStruct::ComponentsSet c)
{
    const ECStruct::ComponentType componentType = ECStruct::ComponentType(((int)c - ECStruct::enumOffset) / ECStruct::enumStride);

    switch(componentType)
    {
    case ECStruct::ComponentType::Transform:
    {
        Qt3DCore::QTransform *transform = new Qt3DCore::QTransform(entityItem->entity);
        TransformWidget *w = new TransformWidget(transform, contentsArea);
        addComponent(transform, c, w);
        return;
    }
    case ECStruct::ComponentType::Material:
    {
        createMaterialComponent(c);
        return;
    }
    case ECStruct::ComponentType::Mesh:
    {
        createMeshComponent(c);
        return;
    }
    default:
        return;
    }
}

#include <Qt3DExtras/QDiffuseMapMaterial>
#include <Qt3DExtras/QDiffuseSpecularMapMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QGoochMaterial>
#include <Qt3DExtras/QMetalRoughMaterial>
#include <Qt3DExtras/QMorphPhongMaterial>
#include <Qt3DExtras/QNormalDiffuseMapMaterial>
#include <Qt3DExtras/QNormalDiffuseSpecularMapMaterial>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTextureMaterial>
void ComponentsSettingWidget::createMaterialComponent(const ECStruct::ComponentsSet c)
{
    Qt3DCore::QComponent *material = nullptr;
    AbstractComponentsSettingWidget *widget = nullptr;

    switch(c)
    {
    case ECStruct::ComponentsSet::DiffuseMapMaterial:
    {
        material = new Qt3DExtras::QDiffuseMapMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::DiffuseSpecularMapMaterial:
    {
        material = new Qt3DExtras::QDiffuseSpecularMapMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::DiffuseSpecularMaterial:
    {
        material = new Qt3DExtras::QDiffuseSpecularMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::GoochMaterial:
    {
        material = new Qt3DExtras::QGoochMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::MetalRougthMaterial:
    {
        material = new Qt3DExtras::QMetalRoughMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::MorphPhongMaterial:
    {
        material = new Qt3DExtras::QMorphPhongMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::NormalDiffuseMapMaterial:
    {
        material = new Qt3DExtras::QNormalDiffuseMapMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::NormalDiffuseSpecularMapMaterial:
    {
        material = new Qt3DExtras::QNormalDiffuseSpecularMapMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::PerVertexColorMaterial:
    {
        material = new Qt3DExtras::QPerVertexColorMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::PhongAlphaMaterial:
    {
        material = new Qt3DExtras::QPhongAlphaMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::PhongMaterial:
    {
        material = new Qt3DExtras::QPhongMaterial(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::TextureMaterial:
    {
        material = new Qt3DExtras::QTextureMaterial(entityItem->entity);
        break;
    }
    default:
        return;
    }

    addComponent(material, c, widget);
}

#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
void ComponentsSettingWidget::createMeshComponent(const ECStruct::ComponentsSet c)
{
    Qt3DCore::QComponent *mesh = nullptr;
    AbstractComponentsSettingWidget *widget = nullptr;

    switch(c)
    {
    case ECStruct::ComponentsSet::Cone:
    {
        mesh = new Qt3DExtras::QConeMesh(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::Cuboid:
    {
        mesh = new Qt3DExtras::QCuboidMesh(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::Cylinder:
    {
        mesh = new Qt3DExtras::QCylinderMesh(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::ExtrudedText:
    {
        mesh = new Qt3DExtras::QExtrudedTextMesh(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::Plane:
    {
        mesh = new Qt3DExtras::QPlaneMesh(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::Sphere:
    {
        mesh = new Qt3DExtras::QSphereMesh(entityItem->entity);
        break;
    }
    case ECStruct::ComponentsSet::Torus:
    {
        mesh = new Qt3DExtras::QTorusMesh(entityItem->entity);
        break;
    }
    default:
        return;
    }

    addComponent(mesh, c, widget);
}
























ComponentManager::ComponentManager(QWidget *parent)
    : QWidget(parent)
    , currentEntityItem(nullptr)
    , settingPageStack(new QStackedWidget(this))
{
    setupLayout();
}

void ComponentManager::setupLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    setLayout(vLayout);
    vLayout->addWidget(settingPageStack);

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

    setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
}

void ComponentManager::setCurrentEntityItem(EntityTreeItem *item)
{
    if(settingPageStack->indexOf(item->componentsSetting) == -1)
        settingPageStack->addWidget(item->componentsSetting);
    settingPageStack->setCurrentWidget(item->componentsSetting);
    item->componentsSetting->setParent(settingPageStack);
}



