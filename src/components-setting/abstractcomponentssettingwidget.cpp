#include "abstractcomponentssettingwidget.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QSpacerItem>
AbstractComponentsSettingWidget::AbstractComponentsSettingWidget(Qt3DCore::QComponent *component, const QString& name, QWidget *parent)
    : QWidget(parent)
    , contents(new QWidget(this))
    , targetComponent(component)
    , toolBar(new QToolBar(this))
    , iconLabel(new QLabel(this))
    , label(new QLabel(name, toolBar))
    , expandAction(new QAction(IconSet::play, "expand", toolBar))
    , contractAction(new QAction(IconSet::sort, "contract", toolBar))
    , cloneAction(new QAction(IconSet::clone, "clone", toolBar))
    , removeAction(new QAction(IconSet::trashcan, "remove", toolBar))
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QLabel *spacer = new QLabel(toolBar);

    setLayout(vLayout);
    vLayout->addWidget(toolBar);
    vLayout->addWidget(contents);
    toolBar->addAction(contractAction);
    toolBar->addWidget(iconLabel);
    toolBar->addWidget(label);
    toolBar->addWidget(spacer);
    toolBar->addAction(cloneAction);
    toolBar->addAction(removeAction);

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    toolBar->setIconSize(QSize(iconSize, iconSize));
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);

    setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    contents->setContentsMargins(0, 0, 0, 0);

    connect(expandAction, &QAction::triggered, this, &AbstractComponentsSettingWidget::expandContents);
    connect(contractAction, &QAction::triggered, this, &AbstractComponentsSettingWidget::contractContents);
    connect(cloneAction, &QAction::triggered, this, &AbstractComponentsSettingWidget::requestClone);
    connect(removeAction, &QAction::triggered, this, &AbstractComponentsSettingWidget::requestRemove);
}

void AbstractComponentsSettingWidget::expandContents()
{
    contents->show();
    toolBar->insertAction(expandAction, contractAction);
    toolBar->removeAction(expandAction);
}

void AbstractComponentsSettingWidget::contractContents()
{
    contents->hide();
    toolBar->insertAction(contractAction, expandAction);
    toolBar->removeAction(contractAction);
}

void AbstractComponentsSettingWidget::requestRemove()
{
    emit removeRequested(targetComponent);
}

void AbstractComponentsSettingWidget::requestClone()
{
    emit cloneRequested(this);
}
