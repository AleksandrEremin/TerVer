#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "MainWork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int StartIndex = 0;
    int TableIndex = 0;
    std::vector <int> valuesMW;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonMainRun_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_Interval_cursorPositionChanged(int arg1, int arg2);

    void on_tableWidget_cellActivated(int row, int column);

    double g(double x);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
