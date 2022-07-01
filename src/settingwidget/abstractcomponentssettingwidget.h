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
    explicit AbstractComponentsSettingWidget(Qt3DCore::QComponent *const component, const QString& name, QWidget *parent);

    Qt3DCore::QComponent *const component() const { return targetComponent; }
    virtual AbstractComponentsSettingWidget *const clone() const = 0;

protected:
    QWidget *contents;

private slots:
    void expandContents();
    void contractContents();
    void requestRemove();
    void requestClone();

private:
    Qt3DCore::QComponent *const targetComponent;

    QToolBar *toolBar;
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
