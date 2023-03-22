#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <QPixmap>

class MainWork
{
public:
    int n;                          // Число экспериментов;
    double P;                       // Вероятность;
    double E;                       // Мат. Ожидание (для геометрического распределения);
    double x_ = 0;                  // Выборочное среднее;
    double D = (1 - P) / pow(P, 2); // Дисперсия;
    double S2 = 0;                  // Выборочная дисперсия;
    double Me = 0;                  // Медиана;
    double R = 0;                   // Размах выбора;

    std::map<int, int> values;      // map - хранит (ключ (значение), значение(число повторений));
    std::vector <int> set;       // Храние все n значений, сортируется;


    MainWork(int n,double P_);
    int searchSV();   // Функция розыгрыша;
    void CastValue();  // Розыграть n раз;

    void getE();
    void getx_();
    void getD();
    void getS2();
    void getMe();
    void getR();


    std::map<int, int>::iterator GetIt() {return values.begin();}
    std::map<int, int>::iterator GetItEnd() {return values.end();}


};




