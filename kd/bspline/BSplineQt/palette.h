#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QPaintEvent>
#include <cmath>
#include <QCursor>

namespace Ui {
class Palette;
}

class Palette : public QWidget
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = 0);
    ~Palette();

    bool showCtrlNode;
    bool showCurveNode;
    int currentK;

    QVector<QPointF> ctrlPoints;  // 控制点
    QVector<QPointF> curvePoints; // 曲线上的点

    void generateCurve();         // 生成B样条曲线
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent* event);

private:
    Ui::Palette *ui;

    double N(int k, int i, double u);
    double N1(int i, double u);
    double N2(int i, double u);
    double N3(int i, double u);   // 三次B样条的基函数

    int currentNode;
    int getCurrentNode(QPointF t);
};

#endif // PALETTE_H
