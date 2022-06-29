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









#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolBar>

ComponentManager::ComponentManager(QWidget *parent)
    : QWidget(parent)
    , currentEntityItem(nullptr)
    , componentsMene(new ComponentsMenu(this))
{
    setupLayout();
}

void ComponentManager::setupLayout()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QToolBar *toolBar = new QToolBar(this);

    setLayout(vLayout);

    vLayout->addWidget(toolBar);

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    QAction *addComponentAction = toolBar->addAction(QIcon(QPixmap(":/icon/plus")), "Add component");
    toolBar->setIconSize(QSize(15, 15));

    setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    toolBar->setContentsMargins(0, 0, 0, 0);
    toolBar->layout()->setSpacing(0);
    toolBar->layout()->setContentsMargins(0, 0, 0, 0);

    connect(addComponentAction, &QAction::triggered, this, &ComponentManager::showComponentsMenu);
}

void ComponentManager::setCurrentEntityItem(EntityTreeItem *item)
{
    qDebug() << item->text(0);
}

void ComponentManager::showComponentsMenu()
{
    componentsMene->exec(cursor().pos());
}


