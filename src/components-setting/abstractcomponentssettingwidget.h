#ifndef ABSTRACT_COMPONENTS_SETTING_WIDGET_H
#define ABSTRACT_COMPONENTS_SETTING_WIDGET_H
#include <QWidget>
#include <QToolBar>
#include <QLabel>
#include <Qt3DCore/QComponent>
#include "../scenemanager.h"

class AbstractComponentsSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractComponentsSettingWidget(Qt3DCore::QComponent *const component,
                                             const QString& name,
                                             QWidget *parent,
                                             const bool isSubComponent = false);

public:
    Qt3DCore::QComponent *const component() const { return targetComponent; }
    virtual AbstractComponentsSettingWidget *const clone() const = 0;
    void setIcon(const QIcon& icon) { iconLabel->setPixmap(icon.pixmap(32, 32)); }
    void setParentEntity(Qt3DCore::QEntity *entity) { this->entity = entity; }

protected:
    QWidget *contents;
    static constexpr int iconSize = 15;

private slots:
    void expandContents();
    void contractContents();
    void requestRemove();
    void receiveRemoveEntity(Qt3DCore::QEntity *entity);
    void requestClone();

private:
    bool isSubComponent;
    Qt3DCore::QEntity *entity;
    Qt3DCore::QComponent *const targetComponent;

    QToolBar *toolBar;
    QLabel *iconLabel;
    QLabel *label;

    QAction *expandAction;
    QAction *contractAction;
    QAction *cloneAction;
    QAction *removeAction;

signals:
    void cloneRequested(AbstractComponentsSettingWidget *w);
    void removeRequested(Qt3DCore::QComponent *comp);
};

#endif // ABSTRACT_COMPONENTS_SETTING_WIDGET_H
