#ifndef LAYOUT_H
#define LAYOUT_H
#include <QWidget>
#include <QVector3D>
#include <QDoubleSpinBox>

class QLabel;

class Form3DDoubleSpin : public QWidget
{
    Q_OBJECT
public:
    explicit Form3DDoubleSpin(QWidget *parent);

public:
    void setMinimum(const double& min);
    void setMaximum(const double& max);
    QVector3D value() const { return QVector3D(xSpinBox->value(), ySpinBox->value(), zSpinBox->value()); }

public slots:
    void setXValue(const double& x) { xSpinBox->setValue(x); }
    void setYValue(const double& y) { ySpinBox->setValue(y); }
    void setZValue(const double& z) { zSpinBox->setValue(z); }
    void setValue(const QVector3D& vec)
    {
        xSpinBox->setValue(vec.x());
        ySpinBox->setValue(vec.y());
        zSpinBox->setValue(vec.z());
    }

protected:
    QLabel *xLabel;
    QLabel *yLabel;
    QLabel *zLabel;

    QDoubleSpinBox *xSpinBox;
    QDoubleSpinBox *ySpinBox;
    QDoubleSpinBox *zSpinBox;

private:
    void emitChangedValue();

signals:
    void xValueChanged(const double& x);
    void yValueChanged(const double& y);
    void zValueChanged(const double& z);
    void valueChanged(const QVector3D& vec);
};






#include <QPushButton>
class QColorDialog;

class ColorDialogButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorDialogButton(const QString& text, QWidget *parent);

public slots:
    void setColor(const QColor& color);

private slots:
    void setButtonColor(const QColor& color);

private:
    QColorDialog *colorDialog;

signals:
    void colorChanged(const QColor& color);
};










class DllSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DllSelectorWidget(const QStringList& funcNameList, QWidget *parent);

    enum class DllState { Resolved, FailedToLoad, FailedToResolve, Unloaded };

public slots:
    void receiveDllState(const DllSelectorWidget::DllState& state);
    void changeFuncName(const QString& symbol);

private:
    QLineEdit *dllPathEdit;
    QLineEdit *symbolNameEdit;
    QList<QLabel*> funcNameLabels;
    QPushButton *requestLoadButton;
    QPushButton *requestUnloadButton;

    QPalette validPalette;
    QPalette invalidPalette;

signals:
    void dllPathEdited(const QString& path);
    void symbolNameEdited(const QString& symbol);
    void loadRequested();
    void unloadRequested();
};







#endif // LAYOUT_H
