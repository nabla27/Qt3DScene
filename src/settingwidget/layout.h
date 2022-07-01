#ifndef LAYOUT_H
#define LAYOUT_H
#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QVector3D>
class Form3DDoubleSpin : public QWidget
{
    Q_OBJECT
public:
    explicit Form3DDoubleSpin(QWidget *parent);

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
#include <QColorDialog>
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


#endif // LAYOUT_H
