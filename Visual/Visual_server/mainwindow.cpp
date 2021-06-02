#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<math.h>

bool flag = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{//Рисуем точки

    int N=256; //Допустим, что у нас пять точек
    QVector<double> x(N), y(N); //Массивы координат точек

    //Зададим наши точки
    int j =-1;
    int k =0;
    for(int i=0; i<64;i++){
        if(i%8 == 0)
            j++;
        if((j == 4) || (j == 7))
            y[i]=y[i-8]+40;
        y[i]=20+39*j;
        if((k == 4) || (k == 7))
            x[i] = x[i-1]+40;
        x[i] = 20+39*k;
        k++;
        if(k > 7)
            k=0;

    }
    for (int i= 64; i<128; i++){
        x[i] = -x[i-64];
        y[i] = y[i-64];
    }
    for (int i= 128; i<192; i++){
        x[i] = x[i-128];
        y[i] = -y[i-128];
    }
    for (int i= 192; i<256; i++){
        x[i] = -x[i-192];
        y[i] = -y[i-192];
    }
    QVector<double> len_vec(64);
    double evm_del = 0;
    for(int i=0; i<64; i++){
        len_vec[i] = sqrt(x[i]*x[i]+y[i]*y[i]);
        evm_del += len_vec[i]*4;
    }


    ui->widget->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в widget
    ui->widget->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->widget->graph(0)->setData(x, y);

    ui->widget->graph(0)->setPen(QColor(255, 0, 0, 255));//задаем цвет точки
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);//убираем линии
    //формируем вид точек
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));

    //Подписываем оси Ox и Oy
    ui->widget->xAxis->setLabel("I");
    ui->widget->yAxis->setLabel("Q");

    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(-320, 320);//Для оси Ox
    ui->widget->yAxis->setRange(-320, 320);//Для оси Oy

    //И перерисуем график на нашем widget
    ui->widget->replot();

    //widget_2

    ui->widget_2->clearGraphs();//Если нужно, но очищаем все графики
    ui->widget_2->addGraph();//Добавляем один график в widget

    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->widget_2->graph(0)->setData(x, y);

    ui->widget_2->graph(0)->setPen(QColor(255, 0, 0, 255));//задаем цвет точки
    ui->widget_2->graph(0)->setLineStyle(QCPGraph::lsNone);//убираем линии
    //формируем вид точек
    ui->widget_2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));

    //Подписываем оси Ox и Oy
    ui->widget_2->xAxis->setLabel("I");
    ui->widget_2->yAxis->setLabel("Q");

    //Установим область, которая будет показываться на графике
    ui->widget_2->xAxis->setRange(-320, 320);//Для оси Ox
    ui->widget_2->yAxis->setRange(-320, 320);//Для оси Oy

    //И перерисуем график на нашем widget
    ui->widget_2->replot();


    int client;
    struct sockaddr_in addr;
    int buf[140][4];
    int buf1[140][4];
    int buf2[140][4];
    int bytes_read=2;
    const  char *ip="192.168.193.3";
    client = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    bind(client, (struct sockaddr *)&addr, sizeof(addr));
    QVector<double> xx, yy, xx1,yy1;
    FILE *f_out;
    f_out = fopen ( "result.txt" , "w" );
    while(bytes_read>1) {
        bytes_read = recvfrom(client, buf, sizeof(buf), 0, NULL, NULL);
        bytes_read = recvfrom(client, buf1, sizeof(buf1), 0, NULL, NULL);
        bytes_read = recvfrom(client, buf2, sizeof(buf2), 0, NULL, NULL);
        double evm_down = 0;
        double evm_up = 0;
        int i,j, num = 0, num1 = 0, num2 = 0;
        for(i=0;i<140;i++){
            for(j=0;j<4;j++){
                if(buf[i][j] != 1000){
                    if(j==0){
                        yy.push_back(buf[i][j]);
                    }
                    if(j==1){
                        xx.push_back(buf[i][j]);
                    }
                    if(j==2){
                        yy1.push_back(buf[i][j]);
                    }
                    if(j==3){
                        xx1.push_back(buf[i][j]);
                    }
                    num++;
                }
                if(buf1[i][j] != 1000){
                    if(j==0){
                        yy.push_back(buf1[i][j]);
                    }
                    if(j==1){
                        xx.push_back(buf1[i][j]);
                    }
                    if(j==2){
                        yy1.push_back(buf1[i][j]);
                    }
                    if(j==3){
                        xx1.push_back(buf1[i][j]);
                    }
                     num1++;
                }
                if(buf2[i][j] != 1000){
                    if(j==0){
                        yy.push_back(buf2[i][j]);
                    }
                    if(j==1){
                        xx.push_back(buf2[i][j]);
                    }
                    if(j==2){
                        yy1.push_back(buf2[i][j]);
                    }
                    if(j==3){
                        xx1.push_back(buf2[i][j]);
                    }
                     num2++;
                }
                if(flag){
                     fprintf(f_out,"%d\n" ,buf1[i][j]);
                }
             }
            int count, count1;
            double min = 10000, min1 = 100000;
            for(int k=0; k < 64; k++) {
                double vec = sqrt(xx[i]*xx[i]+yy[i]*yy[i]);
                double vec1 = sqrt(xx1[i]*xx1[i]+yy1[i]*yy1[i]);
                if(vec-len_vec[k] < min) {
                    min = vec-len_vec[k];
                    count = k;
                }
                if(vec1-len_vec[k] < min1) {
                    min1 = vec-len_vec[k];
                    count1 = k;
                }
            }
            evm_down += (abs(xx[i])-abs(x[count]))*(abs(xx[i])-abs(x[count]))  + (abs(yy[i])-abs(y[count]))*(abs(yy[i])-abs(y[count]));
            evm_up += (abs(xx1[i])-abs(x[count1]))*(abs(xx1[i])-abs(x[count1]))  + (abs(yy1[i])-abs(y[count1]))*(abs(yy1[i])-abs(y[count1]));
        }
        if(flag){
            for(int i =0; i<140 ; i++){
                for(int j =0; j<4; j++){
                    fprintf(f_out,"%d\n" ,buf1[i][j]);
                }
            }
        }
        if(flag){
            for(int i =0; i<140 ; i++){
                for(int j =0; j<4; j++){
                    fprintf(f_out,"%d\n" ,buf2[i][j]);
                }
            }
        }

        evm_down = sqrt(evm_down)/(evm_del)*100;
        evm_up = sqrt(evm_up)/(evm_del)*100;
        ui->textEdit->setText(QString::number(evm_down));
        ui->textEdit_2->setText(QString::number(evm_up));
        memset(buf,1000,sizeof(buf));
        //New Graph
        ui->widget->addGraph();
        ui->widget->graph(1)->setData(xx, yy);
        ui->widget->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
        //формируем вид точек
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        ui->widget->replot();


        ui->widget_2->addGraph();
        ui->widget_2->graph(1)->setData(xx1, yy1);
        ui->widget_2->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
        ui->widget_2->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
        //формируем вид точек
        ui->widget_2->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        ui->widget_2->replot();
        sleep(1);
        xx1.clear();
        yy1.clear();
        xx.clear();
        yy.clear();

        ui->widget->removeGraph(1);
        ui->widget_2->removeGraph(1);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    //Рисуем точки

       int N=4; //Допустим, что у нас пять точек
       QVector<double> x(N), y(N); //Массивы координат точек
       x[0] = 181;
       y[0] = 181;
       x[1] = 181;
       y[1] = -181;
       x[2] = -181;
       y[2] = 181;
       x[3] = -181;
       y[3] = -181;
       QVector<double> len_vec(1);
       double evm_del;
       for(int i=0; i<1; i++){
           len_vec[i] = sqrt(x[i]*x[i]+y[i]*y[i]);
           evm_del += len_vec[i]*4;
       }

       ui->widget->clearGraphs();//Если нужно, но очищаем все графики
       //Добавляем один график в widget
       ui->widget->addGraph();
       //Говорим, что отрисовать нужно график по нашим двум массивам x и y
       ui->widget->graph(0)->setData(x, y);

       ui->widget->graph(0)->setPen(QColor(255, 0, 0, 255));//задаем цвет точки
       ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);//убираем линии
       //формируем вид точек
       ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));

       //Подписываем оси Ox и Oy
       ui->widget->xAxis->setLabel("I");
       ui->widget->yAxis->setLabel("Q");

       //Установим область, которая будет показываться на графике
       ui->widget->xAxis->setRange(-320, 320);//Для оси Ox
       ui->widget->yAxis->setRange(-320, 320);//Для оси Oy

       //И перерисуем график на нашем widget
       ui->widget->replot();

       //widget_2

       ui->widget_2->clearGraphs();//Если нужно, но очищаем все графики
       ui->widget_2->addGraph();//Добавляем один график в widget

       //Говорим, что отрисовать нужно график по нашим двум массивам x и y
       ui->widget_2->graph(0)->setData(x, y);

       ui->widget_2->graph(0)->setPen(QColor(255, 0, 0, 255));//задаем цвет точки
       ui->widget_2->graph(0)->setLineStyle(QCPGraph::lsNone);//убираем линии
       //формируем вид точек
       ui->widget_2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus , 5));

       //Подписываем оси Ox и Oy
       ui->widget_2->xAxis->setLabel("I");
       ui->widget_2->yAxis->setLabel("Q");

       //Установим область, которая будет показываться на графике
       ui->widget_2->xAxis->setRange(-320, 320);//Для оси Ox
       ui->widget_2->yAxis->setRange(-320, 320);//Для оси Oy

       //И перерисуем график на нашем widget
       ui->widget_2->replot();


       int client;
       struct sockaddr_in addr;
       int buf[140][4];
       int buf1[140][4];
       int bytes_read=2;
       const  char *ip="192.168.193.3";
       client = socket(AF_INET, SOCK_DGRAM, 0);
       addr.sin_family = AF_INET;
       addr.sin_port = htons(3425);
       addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
       bind(client, (struct sockaddr *)&addr, sizeof(addr));
       QVector<double> xx, yy, xx1,yy1;
       FILE *f_out;
       f_out = fopen ( "result.txt" , "w" );
       while(bytes_read>1) {
           bytes_read = recvfrom(client, buf, sizeof(buf), 0, NULL, NULL);
           bytes_read = recvfrom(client, buf1, sizeof(buf1), 0, NULL, NULL);
           double evm_down = 0;
           double evm_up = 0;
           int i,j, num = 0, num1 = 0;
           for(i=0;i<140;i++){
               for(j=0;j<4;j++){
                   if(buf[i][j] != 1000){
                       if(j==0){
                           yy.push_back(buf[i][j]);
                       }
                       if(j==1){
                           xx.push_back(buf[i][j]);
                       }
                       if(j==2){
                           yy1.push_back(buf[i][j]);
                       }
                       if(j==3){
                           xx1.push_back(buf[i][j]);
                       }
                       num++;
                   }
                   if(buf1[i][j] != 1000){
                       if(j==0){
                           yy.push_back(buf1[i][j]);
                       }
                       if(j==1){
                           xx.push_back(buf1[i][j]);
                       }
                       if(j==2){
                           yy1.push_back(buf1[i][j]);
                       }
                       if(j==3){
                           xx1.push_back(buf1[i][j]);
                       }
                        num1++;
                   }
                   if(flag){
                        fprintf(f_out,"%d\n" ,buf1[i][j]);
                   }
                }
               int count, count1;
               double min = 10000, min1 = 100000;
               for(int k=0; k < 1; k++) {
                   double vec = sqrt(xx[i]*xx[i]+yy[i]*yy[i]);
                   double vec1 = sqrt(xx1[i]*xx1[i]+yy1[i]*yy1[i]);
                   if(vec-len_vec[k] < min) {
                       min = vec-len_vec[k];
                       count = k;
                   }
                   if(vec1-len_vec[k] < min1) {
                       min1 = vec-len_vec[k];
                       count1 = k;
                   }
               }
               evm_down += (abs(xx[i])-abs(x[count]))*(abs(xx[i])-abs(x[count]))  + (abs(yy[i])-abs(y[count]))*(abs(yy[i])-abs(y[count]));
               evm_up += (abs(xx1[i])-abs(x[count1]))*(abs(xx1[i])-abs(x[count1]))  + (abs(yy1[i])-abs(y[count1]))*(abs(yy1[i])-abs(y[count1]));
           }
           if(flag){
               for(int i =0; i<140 ; i++){
                   for(int j =0; j<4; j++){
                       fprintf(f_out,"%d\n" ,buf1[i][j]);
                   }
               }
           }

           evm_down = sqrt(evm_down)/(evm_del)*100;
           evm_up = sqrt(evm_up)/(evm_del)*100;
           ui->textEdit->setText(QString::number(evm_down));
           ui->textEdit_2->setText(QString::number(evm_up));
           memset(buf,1000,sizeof(buf));
           //New Graph
           ui->widget->addGraph();
           ui->widget->graph(1)->setData(xx, yy);
           ui->widget->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
           ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
           //формируем вид точек
           ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
           ui->widget->replot();


           ui->widget_2->addGraph();
           ui->widget_2->graph(1)->setData(xx1, yy1);
           ui->widget_2->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
           ui->widget_2->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
           //формируем вид точек
           ui->widget_2->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
           ui->widget_2->replot();
           sleep(1);
           xx1.clear();
           yy1.clear();
           xx.clear();
           yy.clear();

           ui->widget->removeGraph(1);
           ui->widget_2->removeGraph(1);
       }
}

void MainWindow::on_pushButton_3_clicked()
{
    //Рисуем точки

       int N=64; //Допустим, что у нас пять точек
       QVector<double> x(N), y(N); //Массивы координат точек
       x[0] = 40;     x[4] = 40;     x[8] = 40;     x[12] = 40;
       y[0] = 40;     y[4] = 119;    y[8] = 197;    y[12] = 277;
       x[1] = 119;    x[5] = 119;    x[9] = 119;    x[13] = 119;
       y[1] = 40;     y[5] = 119;    y[9] = 197;    y[13] = 277;
       x[2] = 197;    x[6] = 197;    x[10] = 197;   x[14] = 197;
       y[2] = 40;     y[6] = 119;    y[10] = 197;   y[14] = 277;
       x[3] = 277;    x[7] = 277;    x[11] = 277;   x[15] = 277;
       y[3] = 40;     y[7] = 119;    y[11] = 197;   y[15] = 277;
       for(int i=16; i< 32;i++){
           x[i] = -x[i-16];
           y[i] = y[i-16];
       }
       for(int i=32; i< 48;i++){
           x[i] = x[i-32];
           y[i] = -y[i-32];
       }
       for(int i=48; i< 64;i++){
           x[i] = -x[i-48];
           y[i] = -y[i-48];
       }
       QVector<double> len_vec(16);
       double evm_del;
       for(int i=0; i<16; i++){
           len_vec[i] = sqrt(x[i]*x[i]+y[i]*y[i]);
           evm_del += len_vec[i]*4;
       }

       ui->widget->clearGraphs();//Если нужно, но очищаем все графики
       //Добавляем один график в widget
       ui->widget->addGraph();
       //Говорим, что отрисовать нужно график по нашим двум массивам x и y
       ui->widget->graph(0)->setData(x, y);

       ui->widget->graph(0)->setPen(QColor(255, 0, 0, 255));//задаем цвет точки
       ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);//убираем линии
       //формируем вид точек
       ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus , 5));

       //Подписываем оси Ox и Oy
       ui->widget->xAxis->setLabel("I");
       ui->widget->yAxis->setLabel("Q");

       //Установим область, которая будет показываться на графике
       ui->widget->xAxis->setRange(-320, 320);//Для оси Ox
       ui->widget->yAxis->setRange(-320, 320);//Для оси Oy

       //И перерисуем график на нашем widget
       ui->widget->replot();

       //widget_2

       ui->widget_2->clearGraphs();//Если нужно, но очищаем все графики
       ui->widget_2->addGraph();//Добавляем один график в widget

       //Говорим, что отрисовать нужно график по нашим двум массивам x и y
       ui->widget_2->graph(0)->setData(x, y);

       ui->widget_2->graph(0)->setPen(QColor(255, 0, 0,255));//задаем цвет точки
       ui->widget_2->graph(0)->setLineStyle(QCPGraph::lsNone);//убираем линии
       //формируем вид точек
       ui->widget_2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus , 5));

       //Подписываем оси Ox и Oy
       ui->widget_2->xAxis->setLabel("I");
       ui->widget_2->yAxis->setLabel("Q");

       //Установим область, которая будет показываться на графике
       ui->widget_2->xAxis->setRange(-320, 320);//Для оси Ox
       ui->widget_2->yAxis->setRange(-320, 320);//Для оси Oy

       //И перерисуем график на нашем widget
       ui->widget_2->replot();


       int client;
       struct sockaddr_in addr;
       int buf[140][4];
       int buf1[140][4];
       int bytes_read=2;
       const  char *ip="192.168.193.3";
       client = socket(AF_INET, SOCK_DGRAM, 0);
       addr.sin_family = AF_INET;
       addr.sin_port = htons(3425);
       addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
       bind(client, (struct sockaddr *)&addr, sizeof(addr));
       QVector<double> xx, yy, xx1,yy1;
       FILE *f_out;
       f_out = fopen ( "result.txt" , "w" );
       while(bytes_read>1) {
           bytes_read = recvfrom(client, buf, sizeof(buf), 0, NULL, NULL);
           bytes_read = recvfrom(client, buf1, sizeof(buf1), 0, NULL, NULL);
           double evm_down = 0;
           double evm_up = 0;
           int i,j, num = 0, num1 = 0;
           for(i=0;i<140;i++){
               for(j=0;j<4;j++){
                   if(buf[i][j] != 1000){
                       if(j==0){
                           yy.push_back(buf[i][j]);
                       }
                       if(j==1){
                           xx.push_back(buf[i][j]);
                       }
                       if(j==2){
                           yy1.push_back(buf[i][j]);
                       }
                       if(j==3){
                           xx1.push_back(buf[i][j]);
                       }
                       num++;
                   }
                   if(buf1[i][j] != 1000){
                       if(j==0){
                           yy.push_back(buf1[i][j]);
                       }
                       if(j==1){
                           xx.push_back(buf1[i][j]);
                       }
                       if(j==2){
                           yy1.push_back(buf1[i][j]);
                       }
                       if(j==3){
                           xx1.push_back(buf1[i][j]);
                       }
                        num1++;
                   }
                   if(flag){
                        fprintf(f_out,"%d\n" ,buf1[i][j]);
                   }
                }
               int count, count1;
               double min = 10000, min1 = 100000;
               for(int k=0; k < 16; k++) {
                   double vec = sqrt(xx[i]*xx[i]+yy[i]*yy[i]);
                   double vec1 = sqrt(xx1[i]*xx1[i]+yy1[i]*yy1[i]);
                   if(vec-len_vec[k] < min) {
                       min = vec-len_vec[k];
                       count = k;
                   }
                   if(vec1-len_vec[k] < min1) {
                       min1 = vec-len_vec[k];
                       count1 = k;
                   }
               }
               evm_down += (abs(xx[i])-abs(x[count]))*(abs(xx[i])-abs(x[count]))  + (abs(yy[i])-abs(y[count]))*(abs(yy[i])-abs(y[count]));
               evm_up += (abs(xx1[i])-abs(x[count1]))*(abs(xx1[i])-abs(x[count1]))  + (abs(yy1[i])-abs(y[count1]))*(abs(yy1[i])-abs(y[count1]));
           }
           if(flag){
               for(int i =0; i<140 ; i++){
                   for(int j =0; j<4; j++){
                       fprintf(f_out,"%d\n" ,buf1[i][j]);
                   }
               }
           }

           evm_down = sqrt(evm_down)/(evm_del)*100;
           evm_up = sqrt(evm_up)/(evm_del)*100;
           ui->textEdit->setText(QString::number(evm_down));
           ui->textEdit_2->setText(QString::number(evm_up));


           memset(buf,1000,sizeof(buf));
           //New Graph
           ui->widget->addGraph();
           ui->widget->graph(1)->setData(xx, yy);
           ui->widget->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
           ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
           //формируем вид точек
           ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
           ui->widget->replot();


           ui->widget_2->addGraph();
           ui->widget_2->graph(1)->setData(xx1, yy1);
           ui->widget_2->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
           ui->widget_2->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
           //формируем вид точек
           ui->widget_2->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
           ui->widget_2->replot();
           sleep(1);
           xx1.clear();
           yy1.clear();
           xx.clear();
           yy.clear();

           ui->widget->removeGraph(1);
           ui->widget_2->removeGraph(1);
       }
       fclose(f_out);
}

void MainWindow::on_radioButton_clicked()
{

}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if(checked)
        flag = true;
}
