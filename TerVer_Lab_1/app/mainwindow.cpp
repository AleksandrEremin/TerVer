#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidgetMain1->setHorizontalHeaderLabels(QStringList() << "Значение случайной величины" << "Число повторений" << "Частота"<<"Теоретические вероятности");
    ui->tableWidgetMain1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidgetMain1_2->setHorizontalHeaderLabels(QStringList() << "Eη" << "x̅" << "|Eη - x̅|" << "Dη" << "S^2" << "|Dη - S^2|" << "Me^" << "R^");
    ui->tableWidgetMain1_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonMainRun_clicked()
{
    int n = ui->lineEdit_n->text().toDouble();
    double p = ui->lineEdit_p->text().toDouble();
    double E = 1/p;            // Мат. Ожидание (для геометрического распределения);
    double x_ = 0;             // Выборочное среднее;
    double D = (1-p)/pow(p,2); // Дисперсия;
    double S2 = 0;             // Выборочная дисперсия;
    double Me = 0;             // Медиана;
    double R = 0;              // Размах выбора;

    MainWork Prog(n,p);
    std::map<int, int> values;

    std::vector <int> set(Prog.n);
    for(int i=0; i<Prog.n; i++)
        set[i]=0;


    srand((unsigned int)time(0));
    for (int i = 0; i <= Prog.n - 1; i++)
    {
        int sv = Prog.searchSV();
        values[sv] += 1;
        set[i] = sv;
    }
    sort(set.begin(), set.end());

    ui->tableWidgetMain1->clearContents();
    ui->tableWidgetMain1->setRowCount(0);
    std::map<int, int>::iterator itr= values.begin();
    int i = 0;
    while(itr != values.end())
    {
        double etta = (*itr).second;
        double frequency = etta / n;
        QTableWidgetItem *value = new QTableWidgetItem(QString::number((*itr).first));
        QTableWidgetItem *repeats = new QTableWidgetItem(QString::number((*itr).second));
        QTableWidgetItem *freq = new QTableWidgetItem(QString::number(frequency));
        double pt = p*(pow((1-p), (*itr).first - 1));
        QTableWidgetItem *PT = new QTableWidgetItem(QString::number(pt));

         x_ += (*itr).first*(*itr).second;


        ui->tableWidgetMain1->insertRow(i);
        ui->tableWidgetMain1->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
        ui->tableWidgetMain1->setItem(i, 0, value);
        ui->tableWidgetMain1->setItem(i, 1, repeats);
        ui->tableWidgetMain1->setItem(i, 2, freq);
        ui->tableWidgetMain1->setItem(i, 3, PT);
        i++;
        itr++;
    }
     x_ = x_/Prog.n;
     itr = values.begin();
     while (itr != values.end())
     {
         S2 += pow((*itr).first - x_, 2)*(*itr).second;
         itr++;
     }
     S2 = S2/Prog.n;


     if (Prog.n % 2)
         Me = set[Prog.n / 2];
     else
     {
         Me = (set[Prog.n / 2 - 1] + set[Prog.n / 2]) / 2;
     }

     R = set[n - 1] - set[0];

     QTableWidgetItem *Table_E = new QTableWidgetItem(QString::number(E));
     QTableWidgetItem *Table_x = new QTableWidgetItem(QString::number(x_));
     QTableWidgetItem *Table_Ex = new QTableWidgetItem(QString::number(abs(E-x_)));
     QTableWidgetItem *Table_D = new QTableWidgetItem(QString::number(D));
     QTableWidgetItem *Table_S2 = new QTableWidgetItem(QString::number(S2));
     QTableWidgetItem *Table_DS2 = new QTableWidgetItem(QString::number(abs(D-S2)));
     QTableWidgetItem *Table_Me = new QTableWidgetItem(QString::number(Me));
     QTableWidgetItem *Table_R = new QTableWidgetItem(QString::number(R));

     ui->tableWidgetMain1_2->setItem(0, 0, Table_E);
     ui->tableWidgetMain1_2->setItem(0, 1, Table_x);
     ui->tableWidgetMain1_2->setItem(0, 2, Table_Ex);
     ui->tableWidgetMain1_2->setItem(0, 3, Table_D);
     ui->tableWidgetMain1_2->setItem(0, 4, Table_S2);
     ui->tableWidgetMain1_2->setItem(0, 5, Table_DS2);
     ui->tableWidgetMain1_2->setItem(0, 6, Table_Me);
     ui->tableWidgetMain1_2->setItem(0, 7, Table_R);
}
