#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    palette = new Palette(this);

    setWindowTitle(tr("B样条曲线模拟程序: B-spline Curve"));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->groupBox->setGeometry(width()-230,10,211,height()-50);

    palette->setGeometry(0,0,width()-230,height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lblLink_linkActivated(const QString &link)
{
    //QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_checkBox_clicked()
{
    palette->showCtrlNode = ui->checkBox->isChecked();
    palette->update();
}

void MainWindow::on_checkBox_2_clicked()
{
    palette->showCurveNode = ui->checkBox_2->isChecked();
    palette->update();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->lblInfo->setText(tr("次（即%1阶）").arg(index+2));
    palette->currentK = index+1;

    palette->generateCurve();
    palette->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
        palette->ctrlPoints.pop_back();
        break;
    case Qt::Key_C:
        palette->ctrlPoints.clear();
    default:
        break;
    }

    palette->generateCurve();
    palette->update();
}
