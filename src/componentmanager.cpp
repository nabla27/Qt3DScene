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












#include <QToolBar>
ComponentsSettingWidget::ComponentsSettingWidget(EntityTreeItem *entityItem)
    : QWidget(nullptr)
    , entityItem(entityItem)
    , componentsMenu(new ComponentsMenu(this))
    , entityNameEdit(new QLineEdit(this))
    , entityEnableCheck(new QCheckBox(this))
    , scrollArea(new QScrollArea(this))
    , contentsArea(new QWidget(scrollArea))
    , contentsLayout(new QVBoxLayout(contentsArea))
{
    setupLayout();

    connect(entityItem, &EntityTreeItem::itemChanged, this, &ComponentsSettingWidget::receiveItemChanging);
    connect(entityNameEdit, &QLineEdit::textChanged, this, &ComponentsSettingWidget::setEntityName);
    connect(entityEnableCheck, &QCheckBox::toggled, this, &ComponentsSettingWidget::setEntityEnable);
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
    vLayout->addWidget(scrollArea);
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



