#include "palette.h"
#include "ui_palette.h"
#include <QDebug>
#include <iostream>

Palette::Palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Palette)
{
    ui->setupUi(this);

    currentK = 3;

    showCtrlNode = true;
    showCurveNode = true;

    setMouseTracking(true);
}

void Palette::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(QCursor(Qt::ClosedHandCursor));

        int i = getCurrentNode(event->pos());
        if(i >= 0)
        {
            currentNode = i;
            return;
        }

        ctrlPoints.push_back(event->pos());
        currentNode = ctrlPoints.size()-1;
        generateCurve();
        update();
    }
}

void Palette::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        ctrlPoints.remove(currentNode);
        ctrlPoints.insert(currentNode,event->pos());
        generateCurve();
        update();
        return;
    }

    if(getCurrentNode(event->pos()) >= 0)
    {
        setCursor(QCursor(Qt::SizeAllCursor));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void Palette::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
}


void Palette::generateCurve()
{
    curvePoints.clear();
    for(double u=currentK; u<ctrlPoints.size(); u+=0.01)
    {
        QPointF tmp(0,0);
        for(int i=0; i<ctrlPoints.size();i++)
        {
            QPointF t = ctrlPoints[i];

            t*=N(currentK,i,u);

            tmp+=t;
        }
        curvePoints.push_back(tmp);
    }
}

double Palette::N(int k, int i, double u)
{
    switch(k)
    {
    case 1:
        return N1(i,u);
    case 2:
        return N2(i,u);
    case 3:
        return N3(i,u);
    }
}

double Palette::N1(int i, double u)
{
    double t = u-i;

    if(0<=t && t<1)
        return t;
    if(1<=t && t<2)
        return 2-t;
    return 0;
}

double Palette::N2(int i, double u)
{
    double t = u-i;

    if(0<=t && t<1)
        return 0.5*t*t;
    if(1<=t && t<2)
        return 3*t-t*t-1.5;
    if(2<=t && t<3)
        return 0.5*pow(3-t,2);
    return 0;
}

double Palette::N3(int i, double u)
{
    double t = u-i;
    double a = 1.0/6.0;

    if(0<=t && t<1)
        return a*t*t*t;
    if(1<=t && t<2)
        return a*(-3*pow(t-1,3) + 3*pow(t-1,2) + 3*(t-1) +1);
    if(2<=t && t<3)
        return a*(3*pow(t-2,3) - 6*pow(t-2,2) +4);
    if(3<=t && t<4)
        return a*pow(4-t,3);
    return 0;
}

void Palette::paintEvent(QPaintEvent *event)
{
    QPainter* painter = new QPainter(this);

    // draw Contorl Points
    QPen ctrlPen1(QColor(0,0,255));
    ctrlPen1.setWidth(7);
    painter->setPen(ctrlPen1);
    for(int i=0; i< ctrlPoints.size(); i++)
    {
        painter->drawPoint(ctrlPoints[i]);
    }

    // draw Control Lines;
    QPen ctrlPen2(QColor(255,0,0));
    ctrlPen2.setWidth(1);
    ctrlPen2.setStyle(Qt::DashDotDotLine);
    painter->setPen(ctrlPen2);
    for(int i=0; i<ctrlPoints.size()-1;i++)
    {
        painter->drawLine(ctrlPoints[i],ctrlPoints[i+1]);
    }

    // draw Spline Curve
    QPen curvePen(QColor(0,0,0));
    curvePen.setWidth(2);
    painter->setPen(curvePen);

    for(int i=0; i<curvePoints.size()-1; i++)
    {
        painter->drawLine(curvePoints[i],curvePoints[i+1]);
    }

    // (Optional) show some hint text;
    if(showCtrlNode)
    {
        QPen pen1(QColor(0,0,255));
        painter->setPen(pen1);
        painter->setFont(QFont("Consolas",12));
        for(int i=0; i< ctrlPoints.size(); i++)
        {
            QPointF t(ctrlPoints[i].x()+20,ctrlPoints[i].y());
            painter->drawText(t,QString("P%1").arg(i));
        }
    }

    // (Optional) show some hint text on Curve
    if(showCurveNode)
    {
        QPen pen2(QColor(10,200,50));
        pen2.setWidth(5);
        painter->setPen(pen2);
        painter->setFont(QFont("Consolas",12));
        for(double u=currentK; u<=ctrlPoints.size(); u+=1)
        {
            QPointF tmp(10,20);
            for(int i=0; i<ctrlPoints.size();i++)
            {
                QPointF t = ctrlPoints[i];

                t*=N(currentK,i,u);

                tmp+=t;
            }
            painter->drawText(tmp,QString("u=%1").arg(u));
            painter->drawPoint(tmp.x()-10,tmp.y()-20);
        }
    }

    delete painter;
}

int Palette::getCurrentNode(QPointF t)
{
    for(int i=0; i<ctrlPoints.size(); i++)
    {
        double dx = ctrlPoints[i].x() - t.x();
        double dy = ctrlPoints[i].y() - t.y();

        double length = sqrt(dx*dx+dy*dy);

        if(length<10)
        {
            return i;
        }
    }
    return -1;
}

Palette::~Palette()
{
    delete ui;
}
