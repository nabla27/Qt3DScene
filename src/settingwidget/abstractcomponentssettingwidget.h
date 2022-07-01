#ifndef ABSTRACT_COMPONENTS_SETTING_WIDGET_H
#define ABSTRACT_COMPONENTS_SETTING_WIDGET_H
#include <QWidget>
#include <QToolBar>
#include <QLabel>
#include <Qt3DCore/QComponent>

class AbstractComponentsSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractComponentsSettingWidget(Qt3DCore::QComponent *component, const QString& name, QWidget *parent);

    const Qt3DCore::QComponent *const component() const { return targetComponent; }

protected:
    QWidget *contents;

private slots:
    void expandContents();
    void contractContents();

private:
    Qt3DCore::QComponent *targetComponent;

    QToolBar *toolBar;
    QLabel *label;

    QAction *expandAction;
    QAction *contractAction;
};

#endif // ABSTRACT_COMPONENTS_SETTING_WIDGET_H
