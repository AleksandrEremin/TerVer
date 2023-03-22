#include "MainWork.h"
#include "mainwindow.h"

MainWork::MainWork(int n_,double P_)
{
    n=n_;
    P=P_;

    set.resize(n);
    for (int i = 0; i < n; i++)
        set[i] = 0;
}

void MainWork::getE()
{
    E = 1 / P;
}

void MainWork::getx_()
{
    std::map<int, int>::iterator itr = values.begin();
    while (itr != values.end())
    {
        x_ += (*itr).first * (*itr).second;
        itr++;
    }
    x_ = x_ / n;
}

void MainWork::getD()
{
    D = (1 - P) / pow(P, 2);
}

void MainWork::getS2()
{
    std::map<int, int>::iterator itr = values.begin();
    while (itr != values.end())
    {
        S2 += pow((*itr).first - x_, 2) * (*itr).second;
        itr++;
    }
    S2 = S2 / n;
}

void MainWork::getMe()
{
    if (n % 2)
        Me = set[n / 2];
    else
    {
        double a = set[(n / 2) - 1];
        double b =  set[n / 2];
        Me = (a + b) / 2;
    }
}

void MainWork::getR()
{
    R = set[n - 1] - set[0];
}


int MainWork::searchSV() //«разыграть» значение;
{

    int etta = 1;
    double q1;
    double q;
    q1 = P;
    q = 0;
    while (true)
    {
        double u = double(rand()) / RAND_MAX;
        if (u < q1)
        {
            break;
        }
        else
        {
            etta++;
          //  q = pow((1 - p), etta - 1) * p;
          //  q1 = q1 + q;
        }
    }
    return etta;
}

void MainWork::CastValue()
{    
    for (int i = 0; i <= n - 1; i++)
    {
        int sv = searchSV();
        values[sv] += 1;
        set[i] = sv;
    }
    sort(set.begin(), set.end());
    getE();
    getx_();
    getD();
    getS2();
    getMe();
    getR();
}
