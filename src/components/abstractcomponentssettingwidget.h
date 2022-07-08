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
                                             QWidget *parent);

public:
    Qt3DCore::QComponent *const component() const { return targetComponent; }
    virtual AbstractComponentsSettingWidget *const clone() const = 0;
    void setIcon(const QIcon& icon) { iconLabel->setPixmap(icon.pixmap(32, 32)); }

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
};

#endif // ABSTRACT_COMPONENTS_SETTING_WIDGET_H
