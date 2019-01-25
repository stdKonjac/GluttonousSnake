#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopWidget>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QApplication>
#include <QMessageBox>
#include <QMainWindow>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QSound>
#include <QLabel>
#include <QDebug>
#include <QPoint>
#include <QTimer>
#include <QTime>
#include <Qdial>

class MainWindow;
class Point;

class Point  //定义点
{
    public:
        Point();
        int x,y;
        bool operator == (const Point &A);
        bool operator != (const Point &A);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

        QPoint GetScreenCenter();  //获取屏幕中心

        void CreateMap();   //初始化20×20网格
        void CreateSnake(); //初始化蛇身
        void DrawBoard();   //游戏界面背景绘制函数
        void DrawBody();    //蛇身绘制函数

        void CreateTimer(); //新建计时器
        void StartTimer();  //开始计时
        void StopTimer();   //停止计时

        void CreateFood();  //随机指定食物位置
        void EatFood();     //处理吃掉食物后的计分等功能
        bool CheckEat();    //检查是否吃到了食物
        void DrawFood();    //绘制食物

        void CreateExtraFood(); //随机指定+3分的大仓鼠出现位置
        bool ShowExFood();  //设定大仓鼠出现(消失)概率
        void EatExFood();   //处理吃掉大仓鼠后的计分等功能
        bool CheckExEat();  //检查是否吃到了大仓鼠
        void DrawExFood();  //绘制大仓鼠

        void CreateItemFood();  //随机指定药丸位置，药丸吃掉后身长减半，分数不变
        bool ShowItemFood();    //设定药丸出现(消失)概率
        void EatItemFood();     //处理吃掉药丸后的计分等功能
        bool CheckItemEat();    //检查是否吃到了药丸
        void DrawItemFood();    //绘制药丸
        void setspeedcontrol(); //绘制调速转轮

        bool CheckBodyCrush();  //检查是否头撞到了身体

        void CreateScoreBoard();    //创建计分板
        void DrawScoreBoard();  //绘制计分板
        void DrawScoreBoardTitile();//绘制记分板标题

        void UpMove();  //移动操作:上
        void DownMove();    //移动操作:下
        void LeftMove();    //移动操作:左
        void RightMove();   //移动操作：右
        void keyPressEvent(QKeyEvent *key); //重写按键事件，根据按键移动

        void SetLevel(int level);   //游戏等级（也就是难度）控制
        void UpdateMap();   //每走一步刷新地图，把蛇身所在位置标记，避免食物出现到身体里
        void EndGame();     //结束游戏
        void ResetGame();   //重新开始游戏

        void paintEvent(QPaintEvent *); //重写绘图事件，来绘制游戏界面

    private slots:
        void AutoMove();    //不按键时的自动移动
        void speedchange(); //调速转轮的变换触发

    private:
        bool map[100][100]; //地图网格（实际只有20×20）

        int direction;  //当前移动方向

        int BodyCount;  //身体节数，初始为4节
        Point body[500];    //每节身体的位置
        Point head;     //蛇头位置
        Point tail;     //蛇尾位置

        Point food;     //食物位置
        bool ExistFood;     //食物存在标记
        Point Exfood;       //大仓鼠位置
        bool ExistExFood;   //大仓鼠存在标记
        Point Itemfood;     //药丸位置
        bool ExistItemFood;     //药丸存在标记

        int level;  //游戏等级(难度)
        int score;  //游戏得分
        int DeathTime;  //从开始游戏到现在累积死亡次数
        int BestScore;  //历史最好分数
        QLabel *CurrentScore;   //当前分数，对应显示score
        QLabel *TheBestScore;   //历史最好分数，对应显示BestScore
        QLabel *CurrentDeathTime;   //当前累积死亡次数，对应显示DeathTime
        QLabel *CurrentLevel;   //当前等级（游戏难度），对应显示Level

        QTimer *AutoMoveTimer;  //自动移动计时器，默认0.15s移动一次
        QTimer *KeyPressTimer;  //按键计时器，默认1ms检查一次是否按下
        QDial  *speedcontrol;//调速转轮
};

#endif // MAINWINDOW_H
