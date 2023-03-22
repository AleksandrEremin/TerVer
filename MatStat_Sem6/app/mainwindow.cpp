#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidgetMain1->setHorizontalHeaderLabels(QStringList() << "Значение случайной величины" << "Число повторений" << "Частота" << "Теоретические вероятности");
    ui->tableWidgetMain1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetMain1_2->setHorizontalHeaderLabels(QStringList() << "Eη" << "x̅" << "|Eη - x̅|" << "Dη" << "S^2" << "|Dη - S^2|" << "Me^" << "R^");
    ui->tableWidgetMain1_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->graph->yAxis->setRange(-0.05, 1.2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonMainRun_clicked()
{
    ui->tableWidgetMain1->clearContents();
    ui->tableWidgetMain1->setRowCount(0);
    ui->graph->clearGraphs();

    int n = ui->lineEdit_n->text().toDouble();
    double p = ui->lineEdit_p->text().toDouble();
    double rep, etta;
    QVector<double> x, y;
    MainWork Solver(n, p);

    srand((unsigned int)time(0));
    Solver.CastValue();

    int i = 0;
    std::map<int, int>::iterator itr = Solver.GetIt();
    while (itr != Solver.GetItEnd())
    {
        rep = (*itr).second;
        etta = (*itr).first;
        QTableWidgetItem* value = new QTableWidgetItem(QString::number((*itr).first));
        QTableWidgetItem* repeats = new QTableWidgetItem(QString::number(rep));
        QTableWidgetItem* freq = new QTableWidgetItem(QString::number(rep/n));
        double pt = p * (pow((1 - p), (*itr).first - 1));    // теоритические вероятности;
        QTableWidgetItem* PT = new QTableWidgetItem(QString::number(pt));

        ui->tableWidgetMain1->insertRow(i);
        ui->tableWidgetMain1->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
        ui->tableWidgetMain1->setItem(i, 0, value);
        ui->tableWidgetMain1->setItem(i, 1, repeats);
        ui->tableWidgetMain1->setItem(i, 2, freq);
        ui->tableWidgetMain1->setItem(i, 3, PT);
        i++;
        itr++;
    }

    QTableWidgetItem* Table_E = new QTableWidgetItem(QString::number(Solver.E));
    QTableWidgetItem* Table_x = new QTableWidgetItem(QString::number(Solver.x_));
    QTableWidgetItem* Table_Ex = new QTableWidgetItem(QString::number(abs(Solver.E - Solver.x_)));
    QTableWidgetItem* Table_D = new QTableWidgetItem(QString::number(Solver.D));
    QTableWidgetItem* Table_S2 = new QTableWidgetItem(QString::number(Solver.S2));
    QTableWidgetItem* Table_DS2 = new QTableWidgetItem(QString::number(abs(Solver.D - Solver.S2)));
    QTableWidgetItem* Table_Me = new QTableWidgetItem(QString::number(Solver.Me));
    QTableWidgetItem* Table_R = new QTableWidgetItem(QString::number(Solver.R));

    ui->tableWidgetMain1_2->setItem(0, 0, Table_E);
    ui->tableWidgetMain1_2->setItem(0, 1, Table_x);
    ui->tableWidgetMain1_2->setItem(0, 2, Table_Ex);
    ui->tableWidgetMain1_2->setItem(0, 3, Table_D);
    ui->tableWidgetMain1_2->setItem(0, 4, Table_S2);
    ui->tableWidgetMain1_2->setItem(0, 5, Table_DS2);
    ui->tableWidgetMain1_2->setItem(0, 6, Table_Me);
    ui->tableWidgetMain1_2->setItem(0, 7, Table_R);


    ui->graph->xAxis->setRange(0, Solver.set[Solver.set.size() - 1] + 5);
    double Xst = 0, Y = 0, Xend = 0;
    x.push_back(Xst);
    y.push_back(Y);
    itr = Solver.GetIt();
    i = 0;
  /*  while (itr != Solver.GetItEnd()) // Строим выборочную функцию распределения;
    {
        rep = (*itr).second;
        double frequency;
        if (i == 0) frequency = rep / n;

        if (i != 0)
        {
            Xst = Xend;
            Y += frequency;
            x.push_back(Xst);
            y.push_back(Y);
            frequency = rep / n;
        }

        Xend = (*itr).first;
        x.push_back(Xend);
        y.push_back(Y);

        QPen Pen1;
        ui->graph->addGraph();
        Pen1.setColor(QColor(Qt::green));
        Pen1.setWidthF(2);//ширина линии
        ui->graph->graph(i)->setPen(Pen1);
        ui->graph->graph(i)->addData(x, y);
        x.clear();
        y.clear();
        i++;
        itr++;
        if (itr == Solver.GetItEnd())
        {
            Xst = Xend;
            Y += frequency;
            x.push_back(Xst);
            y.push_back(Y);
            x.push_back(Xst + 8);
            y.push_back(Y);

            ui->graph->addGraph();
            ui->graph->graph(i)->setPen(Pen1);
            ui->graph->graph(i)->addData(x, y);
            ui->graph->replot();
            x.clear();
            y.clear();
        }
    }
    i++;*/
    int j = 0;
    double PT = 0;
    itr = Solver.GetIt();
    Xst = 0, Y = 0, Xend = 0;
    x.push_back(Xst);
    y.push_back(Y);
    while (itr != Solver.GetItEnd()) // Строим теоретическую функцию распределения;
    {

       // rep = (*itr).second;
        //double frequency;
        //if (i == 0) frequency = rep / n;

        if (j != 0)
        {
            Xst = Xend;
            Y = PT;
            x.push_back(Xst);
            y.push_back(Y);
           // PT = rep / n;
        }
        PT += p * (pow((1 - p), (*itr).first - 1));
        Xend = (*itr).first;
        x.push_back(Xend);
        y.push_back(Y);

        ui->graph->addGraph();
        QPen Pen2;
        Pen2.setColor(QColor(Qt::blue));
        Pen2.setWidthF(5);//ширина линии
        ui->graph->graph(i)->setPen(Pen2);
        ui->graph->graph(i)->addData(x, y);
        x.clear();
        y.clear();
        i++;
        j++;
        itr++;
        if (itr == Solver.GetItEnd())
        {
            Xst = Xend;
            Y += PT;
            x.push_back(Xst);
            y.push_back(Y);
            x.push_back(Xst + 8);
            y.push_back(Y);

            ui->graph->addGraph();
            ui->graph->graph(i)->setPen(Pen2);
            ui->graph->graph(i)->addData(x, y);
            ui->graph->replot();
            x.clear();
            y.clear();
        }
    }

  //  while (itr != Solver.GetItEnd()) // Строим функцию распределения;
   // {
       /* x.push_back(1);
        y.push_back(0.2);
        x.push_back(2);
        y.push_back(0.2);

        ui->graph->addGraph();
        QPen Pen2;
        Pen2.setColor(QColor(Qt::red));
        ui->graph->graph(i+1)->setPen(Pen2);
        ui->graph->graph(i+1)->addData(x, y);
        ui->graph->replot();*/
   // }

}
