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

    int n = ui->lineEdit_n->text().toInt();
    double p = ui->lineEdit_p->text().toDouble();
    if(p==0) throw "Error";
    double rep, etta, max = 0;
    QVector<double> x, y;
    MainWork Solver(n, p);
    QVector<double> Yp, Yt;

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
        double tmp;
        tmp = abs((rep/n) - pt);
        if (tmp > max)
            max = tmp;
    }
  //  qDebug() << max;

    ui->lineEdit_Max->setText(QString::number(max));
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

    itr = Solver.GetItEndTP();
    itr--;
    int tmptmp = (*itr).first;
    ui->graph->xAxis->setRange(0, tmptmp + 5);
    double Xst = 0, Y = 0, Xend = 0;
    x.push_back(Xst);
    y.push_back(Y);
    itr = Solver.GetIt();
    i = 0;
    while (itr != Solver.GetItEnd()) // Строим выборочную функцию распределения;
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
        for(int i = Xend - Xst; i>0; i-- )
        Yp.push_back(Y);

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
            x.push_back(Xst + tmptmp + 5);
            y.push_back(Y);

            ui->graph->addGraph();
            ui->graph->graph(i)->setPen(Pen1);
            ui->graph->graph(i)->addData(x, y);
            ui->graph->replot();
            x.clear();
            y.clear();
            Yp.push_back(1);
        }
    }
    i++;
    int j = 0;
    double PT = 0;
    itr = Solver.GetItTP();
    Xst = 0, Y = 0, Xend = 0;
    x.push_back(Xst);
    y.push_back(Y);
    while (itr != Solver.GetItEndTP()) // Строим теоретическую функцию распределения;
    {
        if (j != 0)
        {
            Xst = Xend;
            Y = PT;
            x.push_back(Xst);
            y.push_back(Y);
        }
        PT += p * (pow((1 - p), (*itr).first - 1));
        Xend = (*itr).first;
        x.push_back(Xend);
        y.push_back(Y);
        Yt.push_back(Y);

        ui->graph->addGraph();
        QPen Pen2;
        Pen2.setColor(QColor(Qt::blue));
        Pen2.setWidthF(1);//ширина линии
        ui->graph->graph(i)->setPen(Pen2);
        ui->graph->graph(i)->addData(x, y);
        x.clear();
        y.clear();
        i++;
        j++;
        itr++;
        if (itr == Solver.GetItEndTP())
        {
            Xst = Xend;
            Y = PT;
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
            Yt.push_back(1);
        }
    }
    valuesMW.clear();
    for(int i =0; i<Solver.set.size(); i++)
            valuesMW.push_back(Solver.set[i]);
    double MAX, TMP;
    for(int i = 0; i < Yp.size() && i < Yt.size(); i++)
    {
        TMP = abs(Yp[i] - Yt[i]);
        if(i==0)
            MAX = TMP;
        if(TMP > MAX)
            MAX = TMP;
    }
       ui->lineEdit->setText(QString::number(MAX));
       StartIndex = 1;
       if(TableIndex!=0)
       {
           for(int i=0; i<TableIndex; i++)
           on_tableWidget_cellActivated(i,1);
       }
}

/*QTableWidgetItem* value = new QTableWidgetItem(QString::number((*itr).first));
QTableWidgetItem* repeats = new QTableWidgetItem(QString::number(rep));
QTableWidgetItem* freq = new QTableWidgetItem(QString::number(rep/n));
double pt = p * (pow((1 - p), (*itr).first - 1));    // теоритические вероятности;
QTableWidgetItem* PT = new QTableWidgetItem(QString::number(pt));

ui->tableWidgetMain1->insertRow(i);
ui->tableWidgetMain1->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
ui->tableWidgetMain1->setItem(i, 0, value);
ui->tableWidgetMain1->setItem(i, 1, repeats);
ui->tableWidgetMain1->setItem(i, 2, freq);
ui->tableWidgetMain1->setItem(i, 3, PT);*/
void MainWindow::on_pushButton_clicked()
{
    if(TableIndex!=0)
    {
        for(int i=0; i<TableIndex; i++)
        on_tableWidget_cellActivated(i,1);
    }
}


void MainWindow::on_lineEdit_Interval_cursorPositionChanged(int arg1, int arg2)
{
    if(StartIndex == 1)
   {
        ui->tableWidget->clearContents();
        int intervals = ui->lineEdit_Interval->text().toInt();
        ui->tableWidget->setRowCount(intervals);
        QTableWidgetItem* one = new QTableWidgetItem(QString::number(1));
        ui->tableWidget->setItem(0, 0, one);
        TableIndex = intervals;
    }
}

double Func(double x, double p)
{
    double f = 0;
    for (int i = 1; i < x; i++)
    {
        f += p * pow((1 - p), (i - 1));
    }
    return f;
}
void MainWindow::on_tableWidget_cellActivated(int row, int column)
{
    if(column == 1)
    {
        double p = ui->lineEdit_p->text().toDouble();
        int intervals = ui->lineEdit_Interval->text().toInt();
        int start;
        int tmp = 0;
        double pt =0.0;
        QString check = "-1";


        QTableWidgetItem *item;
        item = ui->tableWidget->item(row,column-1);

        if (NULL != item)
        {
            check = item->text();
            start  = check.toInt();

            int end;
            item = ui->tableWidget->item(row,column);

            if (NULL != item)
            {
                check = item->text();
                if(row != intervals - 1)
                end  = check.toInt();
            } //1. добавил скобки;

            int tmp;
            int index=0;


            if(row == intervals - 1) end = valuesMW[valuesMW.size()-1] + 10;

            for(int i = 0; i < valuesMW.size(); i++)
            {
                if(valuesMW[i]>=start &&valuesMW[i]<end)
                {
                    index++;
                   // if(tmp != valuesMW[i] || tmp == 0)
                    //{
                        //pt+= p * (pow((1 - p), valuesMW[i] - 1));
                    //}
                    //tmp = valuesMW[i];
                }
            }
            pt = Func(end,p) - Func(start,p);

            QTableWidgetItem* TableIndex = new QTableWidgetItem(QString::number(index));
            QTableWidgetItem* Next = new QTableWidgetItem(QString::number(end));
            ui->tableWidget->setItem(row, column + 1, TableIndex);
            ui->tableWidget->setItem(row + 1,0, Next);

            if(row == intervals - 1)
            {
                QTableWidgetItem* Inf = new QTableWidgetItem("inf");
                ui->tableWidget->setItem(row, column, Inf);
            }

            item = ui->tableWidget->item(row,column);
            if (NULL != item)
                check = item->text();
            if(check == "inf")
                pt = 1 - Func(start,p);
            QTableWidgetItem* PT = new QTableWidgetItem(QString::number(pt));
            ui->tableWidget->setItem(row, column + 2, PT);

        }
        if(row == intervals - 2)
            on_tableWidget_cellActivated(row + 1,1);
    }
}
double G(double a)
{
    if (a == 1)
        return 1;
    else
        return (a - 1) * G(a - 1);
}

double MainWindow::g(double x)
{
    int intervals_k = ui->lineEdit_Interval->text().toInt();
    double e = 2.7182818285;
    return pow(2, -(intervals_k - 1) / 2) * (pow(G((intervals_k - 1) / 2), -1)) * pow(x, (intervals_k - 1) / 2 - 1) * pow(e, -x / 2);
}






void MainWindow::on_pushButton_2_clicked()
{
    double Ro = 0.0;
    int intervals_k = ui->lineEdit_Interval->text().toDouble();
    int n = ui->lineEdit_n->text().toInt();
    int nj;
    double qj;
    int row = 0;
    double tmp;
    double alfa= ui->lineEdit_2->text().toDouble();
    QTableWidgetItem *item;
    QString check;
    for(row = 0; row < intervals_k; row++)
    {
        item = ui->tableWidget->item(row,2);
        check = item->text();
        nj  = check.toInt();

        item = ui->tableWidget->item(row,3);
        check = item->text();
        qj  = check.toDouble();

        qDebug() << nj<<" "<<qj;
        tmp = (nj - n * qj) * (nj - n * qj);
        Ro += tmp / (n * qj);

        qDebug() << Ro;
    }
       ui->lineEdit_3->setText(QString::number(Ro));

       double a = 0; double b = Ro; double summ = 0; int N = 1000;
       for (int i = 1; i<N; i++)
       {
           summ += (g(a + (b - a)*(i - 1)/N) + g(a + (b - a)*(i)/N)) * (b - a)/(2*N);
       }

       ui->lineEdit_4->setText(QString::number((1 - summ)));


       qDebug()<<"----> "<<alfa;
       if ((1 - summ) < alfa)
            ui->lineEdit_5->setText(("отклоняем"));
       else
            ui->lineEdit_5->setText(("принимаем"));

}

