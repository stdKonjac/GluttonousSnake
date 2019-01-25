/*
游戏简介：
①在开始界面，点击“开始游戏”，游戏开始；点击“退出游戏”，游戏结束
②在游戏中，有3种食物，分别是：
    1、普通食物，吃完+1分
    2、大仓鼠，吃完+3分，在等级大于等于3时以20%概率出现和消失
    3、药丸，吃完不加分，但是身体会缩短一半，在等级等于4时以10%概率出现和消失
③游戏会随着分数的增加而提升等级，也即游戏难度，难度越高，移动速度越快
④蛇可以穿墙，也就是说除了撞到自己身体之外都不会死掉掉
⑤游戏中按ESC键可以暂停游戏或者退出游戏
*/

#include "MainWindow.h"
#include "StartWindow.h"
#include <QApplication>
#include <QSound>

QSound BGM(":/sound/bgm.wav");
QSound EatSound(":/sound/eat.wav");
QSound GameOverSound(":/sound/gameover.wav");

void playEatSound()
{
    EatSound.play();
}

void playGameOverSound()
{
    GameOverSound.play();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPoint ScreenCenter;

    /*绘制开始界面*/
    StartWindow startWindow;
    startWindow.setWindowTitle("贪吃蛇");
    startWindow.setFixedSize(750,460);
    ScreenCenter=startWindow.GetScreenCenter();
    startWindow.move(ScreenCenter);

    /*绘制游戏界面*/

    MainWindow GameWindow;

    QSound bgm(":/sound/bgm.wav");
    bgm.setLoops(-1);
    bgm.play();

    GameWindow.setWindowTitle("贪吃蛇");
    GameWindow.setFixedSize(800,600);
    ScreenCenter=GameWindow.GetScreenCenter();
    GameWindow.move(ScreenCenter);
    /*点击开始游戏按钮，发送开始信号，开始游戏界面关闭，游戏界面打开*/
    QDesktopWidget::connect(&startWindow,&StartWindow::StartSignal,&GameWindow,&MainWindow::StartTimer);
    QDesktopWidget::connect(&startWindow,&StartWindow::StartSignal,&GameWindow,&MainWindow::show);


    startWindow.show();  //展示开始界面
    return app.exec();
}
