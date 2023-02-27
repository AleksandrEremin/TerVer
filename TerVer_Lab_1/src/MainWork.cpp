#include "MainWork.h"
#include "mainwindow.h"

MainWork::MainWork(int n_,double p_)
{
    n=n_;
    p=p_;
}

int MainWork::searchSV() //«разыграть» значение;
{

    int etta = 1;
    double q1;
    double q;
    q1 = p;
    q = 0;
    double u = double(rand()) / RAND_MAX;
    while (true)
    {

        if (u < q1) {
            break;
        }
        else {
            etta++;
            q = pow((1 - p), etta - 1) * p;
            q1 = q1 + q;

        }

    }
    return etta;
}
