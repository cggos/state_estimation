#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <palette.h>
#include <QDesktopServices>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Palette* palette;   // 画板

protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void on_lblLink_linkActivated(const QString &link);
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_comboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
