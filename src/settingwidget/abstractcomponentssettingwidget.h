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
    void setIcon(const QString& path) { iconLabel->setPixmap(path); }

protected:
    QWidget *contents;
    static constexpr int iconSize = 15;

private slots:
    void expandContents();
    void contractContents();
    void requestRemove();
    void requestClone();

private:
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
    void loadIconRequested(const QString& path);
};

#endif // ABSTRACT_COMPONENTS_SETTING_WIDGET_H
