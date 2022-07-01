#include "abstractcomponentssettingwidget.h"


#include <QVBoxLayout>
#include <QApplication>
#include <QStyle>
AbstractComponentsSettingWidget::AbstractComponentsSettingWidget(Qt3DCore::QComponent *component, const QString& name, QWidget *parent)
    : QWidget(parent)
    , contents(new QWidget(this))
    , targetComponent(component)
    , toolBar(new QToolBar(this))
    , label(new QLabel(name, toolBar))
    , expandAction(new QAction(QIcon(QPixmap(":icon/play")), "expand"))
    , contractAction(new QAction(QIcon(QPixmap(":/icon/sort")), "contract"))
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    setLayout(vLayout);
    vLayout->addWidget(toolBar);
    vLayout->addWidget(contents);
    toolBar->addAction(contractAction);
    toolBar->addWidget(label);

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    toolBar->setIconSize(QSize(15, 15));

    setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    contents->setContentsMargins(0, 0, 0, 0);

    connect(expandAction, &QAction::triggered, this, &AbstractComponentsSettingWidget::expandContents);
    connect(contractAction, &QAction::triggered, this, &AbstractComponentsSettingWidget::contractContents);
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
