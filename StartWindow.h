#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDesktopWidget>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QLabel>
#include <QFont>
#include "button.h"

class StartWindow : public QMainWindow
{
    Q_OBJECT

    public:
        StartWindow(QWidget *parent=0);
        ~StartWindow();

        QPoint GetScreenCenter();   //获取屏幕标题

        void setStartButton();  //设定开始按钮
        void setExitButton();   //设定退出按钮
        void setGameTitle();    //设定游戏标题
        void paintEvent(QPaintEvent *); //绘制开始界面

        void SendStartSignal(); //发出开始信号

    signals:
        void StartSignal(); //开始信号

    private:
        Button *StartButton;   //开始按钮
        Button *ExitButton;    //退出按钮
        QLabel *GameTitle;  //游戏标题

};

#endif // STARTWINDOW_H
