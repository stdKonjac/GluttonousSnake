#include "MainWindow.h"

Point::Point()
{

}

bool Point::operator == (const Point &A)
{
    return (A.x==this->x&&A.y==this->y)?true:false;
}

bool Point::operator != (const Point &A)
{
    return (A.x!=this->x||A.y!=this->y)?true:false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    direction=4;    //初始化方向向右

    level=1;    //初始化等级为1
    score=0;    //初始化得分为0
    DeathTime=0;    //初始化累积死亡次数为0
    BestScore=0;    //初始化历史最佳得分为0

    ExistFood=false;    //初始食物不存在
    ExistExFood=false;  //初始大仓鼠不存在
    ExistItemFood=false;    //初始药丸不存在

    CreateMap();    //初始化地图
    CreateSnake();  //初始化蛇身
    CreateFood();   //初始化食物
    CreateScoreBoard(); //初始化计分板
    CreateTimer();  //初始化计时器
    setspeedcontrol();//初始化调速转轮
}

MainWindow::~MainWindow()
{

}

QPoint MainWindow::GetScreenCenter()    //获取屏幕中心用于窗口显示位置设置
{
    int X=(QApplication::desktop()->width() - this->width())/2;
    int Y=(QApplication::desktop()->height() - this->height())/2;
    QPoint ScreenCenter;
    ScreenCenter.setX(X);
    ScreenCenter.setY(Y);
    return ScreenCenter;
}

void MainWindow::CreateMap()    //初始化地图
{
    for(int i=0;i<=20;i++)
        for(int j=0;j<=20;j++)
            map[i][j]=false;    //最开始每个格子都是空的
}

void MainWindow::CreateSnake()  //初始化蛇身
{
    BodyCount=4;    //最开始四节，位置如下
    body[1].x=120,body[1].y=120;
    body[2].x=90,body[2].y=120;
    body[3].x=60,body[3].y=120;
    body[4].x=30,body[4].y=120;
    head=body[1];   //头永远是body[1]
    tail=body[BodyCount]; //尾巴永远是body[BodyCount]
    UpdateMap();    //更新地图
}

void MainWindow::DrawBoard()    //画游戏背景
{
    QPainter BoardPainter(this);
    QPixmap BKPic(":/image/images/1397675-0a714724dde3600c.jpg");
    BoardPainter.drawPixmap(0,0,600,600,BKPic);
}

void MainWindow::DrawBody() //画蛇身，此处友情感谢北京理工大学DCC&电子科技大学LCR同学帮我们画了蛇头/身/尾……
{
    /*绘制头部*/
    QPainter HeadPainter(this);
    QPixmap Head(":/image/head.png");
    QMatrix HeadMatrix; //判断头的方向，图片中为向上，根据方向旋转即可
    if(direction==1)
    {
        HeadMatrix.rotate(0.0);
    }
    if(direction==2)
    {
        HeadMatrix.rotate(180.0);
    }
    if(direction==3)
    {
        HeadMatrix.rotate(270.0);
    }
    if(direction==4)
    {
        HeadMatrix.rotate(90.0);
    }
    Head=Head.transformed(HeadMatrix,Qt::FastTransformation);   //旋转图片
    HeadPainter.drawPixmap(head.x,head.y,30,30,Head);
    /*绘制身体*/
    QPainter BodyPainter(this);
    QPixmap Body(":/image/body.png");
    for(int i=2;i<=BodyCount-1;i++)
    {
        BodyPainter.drawPixmap(body[i].x,body[i].y,30,30,Body);
    }
    /*绘制尾巴*/
    QPainter TailPainter(this);
    QPixmap Tail(":/image/tail.png");
    QMatrix TailMatrix; //判断尾巴摆动的方向，默认向下，根据尾巴前一块身体的位置即可进行旋转
    if(body[BodyCount-1].x==body[BodyCount].x+30)
    {
        TailMatrix.rotate(270.0);
    }
    if(body[BodyCount-1].x==body[BodyCount].x-30)
    {
        TailMatrix.rotate(90.0);
    }
    if(body[BodyCount-1].y==body[BodyCount].y+30)
    {
        TailMatrix.rotate(0.0);
    }
    if(body[BodyCount-1].y==body[BodyCount].y-30)
    {
        TailMatrix.rotate(180.0);
    }
    Tail=Tail.transformed(TailMatrix,Qt::FastTransformation);   //旋转尾巴方向
    TailPainter.drawPixmap(body[BodyCount].x,body[BodyCount].y,30,30,Tail);
}

void MainWindow::CreateTimer()  //初始化计时器
{
    AutoMoveTimer=new QTimer(this);
    KeyPressTimer=new QTimer(this);
}

void MainWindow::StartTimer()   //连接自动移动函数及重绘函数，开始计时
{
    connect(AutoMoveTimer,SIGNAL(timeout()),this,SLOT(AutoMove()));
    connect(KeyPressTimer,SIGNAL(timeout()),this,SLOT(update()));
    AutoMoveTimer->start(150);  //默认自动移动频率为0.15s/次
    KeyPressTimer->start(16);    //默认按键确认频率为100ms/次
}

void MainWindow::StopTimer()    //停止计时，解除连接
{
    disconnect(AutoMoveTimer,SIGNAL(timeout()),this,SLOT(AutoMove()));
    disconnect(KeyPressTimer,SIGNAL(timeout()),this,SLOT(update()));
    AutoMoveTimer->stop();
    KeyPressTimer->stop();
}

void MainWindow::CreateFood()   //随机指定食物位置
{
    if(!ExistFood)  //如果食物不存在就创建食物
    {
        qsrand(time(NULL));
        while(1)
        {
            /*随机化食物位置*/
            food.x=qrand()%20;
            food.y=qrand()%20;
            food.x*=30;
            food.y*=30;
          /*  if(food.x+30>=600||food.y+30>=600)
            {
                continue;
            }*/
            /*判断食物是否在空格子里*/
            if(!map[food.x/30][food.y/30])
            {
                if(ExistExFood&&ExistItemFood)
                {
                    if(food!=Exfood&&food!=Itemfood)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(ExistExFood&&!ExistItemFood)
                {
                    if(food!=Exfood)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!ExistExFood&&ExistItemFood)
                {
                    if(food!=Itemfood)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!ExistExFood&&!ExistItemFood)
                {
                    break;
                }
            }
        }
        ExistFood=true; //食物存在标记为true
    }
}

void MainWindow::EatFood()  //处理吃掉食物
{
    body[++BodyCount]=tail; //身长增加，尾巴位置更新

    score++;    //得分增加
    if((score/10+1)>level){
        level=score/10+1;//更新等级
        SetLevel(level);//根据等级设定难度
    }

    if(score>BestScore) //更新历史最好分数
    {
        BestScore=score;
    }
    ExistFood=false;    //食物存在标记为false
}

bool MainWindow::CheckEat() //判断是否吃到了食物
{
    if(head==food)
    {
        QSound::play(":/sound/eat.wav");
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::DrawFood() //绘制食物
{
    CreateFood();
    QPainter FoodPainter(this);
    QPixmap FoodPic(":/image/frog.png");
    FoodPainter.drawPixmap(food.x,food.y,30,30,FoodPic);
}

void MainWindow::CreateExtraFood()  //随机指定大仓鼠位置
{
    if(!ExistExFood&&level>=3)  //大仓鼠不存在且游戏等级为3时出现
    {
        qsrand(time(NULL));
        while(1)
        {
            /*随机化大仓鼠位置*/
            Exfood.x=qrand()%20;
            Exfood.y=qrand()%20;
            Exfood.x*=30;
            Exfood.y*=30;
           /* if(Exfood.x+30>=600||Exfood.y+30>=600)
            {
                continue;
            }*/
            /*判断大仓鼠是否在空余位置*/
            if(!map[Exfood.x/30][Exfood.y/30])
            {
                if(ExistFood&&ExistItemFood)
                {
                    if(Exfood!=food&&Exfood!=Itemfood)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(ExistFood&&!ExistItemFood)
                {
                    if(Exfood!=food)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!ExistFood&&ExistItemFood)
                {
                    if(Exfood!=Itemfood)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!ExistExFood&&!ExistItemFood)
                {
                    break;
                }
            }
        }
        ExistExFood=true;   //大仓鼠存在标记为true
    }
}

bool MainWindow::ShowExFood()   //设定大仓鼠出现(消失)概率
{
    qsrand(time(NULL));
    int seed=qrand()%10; //设定概率为20%
    if(seed==0||seed==1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void MainWindow::EatExFood()    //处理吃掉食物
{
    body[++BodyCount]=tail; //身长+1 更新尾巴位置

    score+=3;   //大仓鼠+3分
    if((score/10+1)>level){
        level=score/10+1;//更新等级
        SetLevel(level);//根据等级设定难度
    }

    if(score>BestScore) //更新历史最佳分数
    {
        BestScore=score;
    }
    ExistExFood=false;  //大仓鼠存在标记为false

}
bool MainWindow::CheckExEat()   //判断是否吃到了大仓鼠
{
    if(head==Exfood)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::DrawExFood()   //绘制大仓鼠
{
    CreateExtraFood();
    QPainter ExFoodPainter(this);
    QPixmap ExFoodPic(":/image/mouse.png");
    ExFoodPainter.drawPixmap(Exfood.x,Exfood.y,30,30,ExFoodPic);
}

void MainWindow::CreateItemFood()   //随机指定药丸出现位置
{
    if(!ExistItemFood&&level==4)    //药丸不存在且游戏难度大于等于4时出现药丸
    {
        qsrand(time(NULL));
        while(1)
        {
            /*随机药丸出现位置*/
            Itemfood.x=qrand()%20;
            Itemfood.y=qrand()%20;
            Itemfood.x*=30;
            Itemfood.y*=30;
           /* if(Itemfood.x+30>=600||Itemfood.y+30>=600)
            {
                continue;
            }*/
            /*判断药丸是否出现在空余位置*/
            if(!map[Itemfood.x/30][Itemfood.y/30])
            {
                if(ExistExFood&&ExistFood)
                {
                    if(Itemfood!=Exfood&&Itemfood!=food)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(ExistExFood&&!ExistFood)
                {
                    if(Itemfood!=Exfood)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!ExistExFood&&ExistFood)
                {
                    if(Itemfood!=food)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if(!ExistExFood&&!ExistItemFood)
                {
                    break;
                }
            }
        }
        ExistItemFood=true; //药丸存在标记为true
    }
}

bool MainWindow::ShowItemFood() //设定药丸出现(消失)概率
{
    qsrand(time(NULL));
    int seed=qrand()%10;    //默认药丸出现(消失)概率为10%
    if(seed==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void MainWindow::EatItemFood()  //处理吃掉药丸
{
    BodyCount/=2;   //身体长度减半
    BodyCount=std::max(BodyCount,4);    //如果身体长度小于4 则置为4
    tail=body[BodyCount];   //更新尾巴位置
    if((score/10+1)>level){
        level=score/10+1;//更新等级
        SetLevel(level);//根据等级设定难度
    }

    if(score>BestScore) //更新历史最佳分数
    {
        BestScore=score;
    }
    ExistItemFood=false;    //药丸存在标记为false
}

bool MainWindow::CheckItemEat() //检查是否吃掉了药丸
{
    if(head==Itemfood)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::DrawItemFood() //绘制药丸
{
    if(!ExistItemFood)
        CreateItemFood();
    QPainter ItemFoodPainter(this);
    QPixmap ItemFoodPic(":/image/item.png");
    ItemFoodPainter.drawPixmap(Itemfood.x,Itemfood.y,30,30,ItemFoodPic);
}

bool MainWindow::CheckBodyCrush()   //检查头是否和身体撞了
{
    for(int i=2;i<=BodyCount;i++)
    {
        if(head==body[i])
        {
            return true;
        }
    }
    return false;
}

void MainWindow::CreateScoreBoard() //初始化计分板
{
    CurrentScore=new QLabel(this);
    TheBestScore=new QLabel(this);
    CurrentDeathTime=new QLabel(this);
    CurrentLevel=new QLabel(this);
}

void MainWindow::DrawScoreBoard()   //绘制计分板
{
    /*绘制计分板背景*/
    QPainter ScoreBoardPainter(this);
    QPixmap BKPic(":/image/ScoreBoardBG.png");
    ScoreBoardPainter.drawPixmap(600,0,200,600,BKPic);

    /*绘制计分板标题
    ScoreBoardTitle->setText("计分板");
    ScoreBoardTitle->setGeometry(630,-20,160,120);
    QFont ScoreBoardTitleFont;
    ScoreBoardTitleFont.setFamily("华文楷体");
    ScoreBoardTitleFont.setPointSize(30);
    ScoreBoardTitle->setFont(ScoreBoardTitleFont);
    QPalette ScoreBoardTitlePalette;
    ScoreBoardTitlePalette.setColor(QPalette::WindowText,Qt::blue);
    ScoreBoardTitle->setPalette(ScoreBoardTitlePalette);
    ScoreBoardTitle->show();
    绘制计分板当前得分并且实时更新*/
    CurrentScore->setText(QString("当前得分: %1").arg(QString::number(score)));
    CurrentScore->setGeometry(630,60,170,100);
    QFont CurrentScoreFont;
    CurrentScoreFont.setFamily("Monaco");
    CurrentScoreFont.setPointSize(15);
    CurrentScore->setFont(CurrentScoreFont);
    QPalette CurrentScorePalette;
    CurrentScorePalette.setColor(QPalette::WindowText,Qt::black);
    CurrentScore->setPalette(CurrentScorePalette);
    CurrentScore->show();
    /*绘制计分板历史最高纪录并且实时更新*/
    TheBestScore->setText(QString("历史记录: %1").arg(QString::number(BestScore)));
    TheBestScore->setGeometry(630,140,170,100);
    QFont TheBestScoreFont;
    TheBestScoreFont.setFamily("Monaco");
    TheBestScoreFont.setPointSize(15);
    TheBestScore->setFont(TheBestScoreFont);
    QPalette TheBestScorePalette;
    TheBestScorePalette.setColor(QPalette::WindowText,Qt::black);
    TheBestScore->setPalette(TheBestScorePalette);
    TheBestScore->show();
    /*绘制计分板累积死亡次数*/
    CurrentDeathTime->setText(QString("死亡次数: %1").arg(QString::number(DeathTime)));
    CurrentDeathTime->setGeometry(630,215,170,100);
    QFont CurrentDeathTimeFont;
    CurrentDeathTimeFont.setFamily("Monaco");
    CurrentDeathTimeFont.setPointSize(15);
    CurrentDeathTime->setFont(CurrentDeathTimeFont);
    QPalette  CurrentDeathTimePalette;
    CurrentDeathTimePalette.setColor(QPalette::WindowText,Qt::black);
    CurrentDeathTime->setPalette(CurrentDeathTimePalette);
    CurrentDeathTime->show();
    /*绘制计分板当前等级并实时更新*/
    CurrentLevel->setText(QString("当前等级:Lv.%1").arg(QString::number(level)));
    CurrentLevel->setGeometry(620,290,170,100);
    QFont CurrentLevelFont;
    CurrentLevelFont.setFamily("Monaco");
    CurrentLevelFont.setPointSize(15);
    CurrentLevel->setFont(CurrentLevelFont);
    QPalette  CurrentLevelPalette;
    CurrentLevelPalette.setColor(QPalette::WindowText,Qt::black);
    CurrentLevel->setPalette(CurrentLevelPalette);
    CurrentLevel->show();
}

void MainWindow::DrawScoreBoardTitile()
{
    QPainter ScoreBoard(this);
    QPixmap pic1(":/image/images/banzi.png");
    ScoreBoard.drawPixmap(616,0,170,74,pic1);
}

void MainWindow::UpMove()   //向上操作
{
    if(direction==2)    //如果此时在向下移动，则按键无效
    {
        return;
    }
    tail=body[BodyCount];   //更新尾部位置
    head.y-=30; //头向下移动一格
    if(head.y<0)    //穿墙
    {
        head.y=570;
    }
    if(CheckBodyCrush())    //如果头撞到身子就凉了
    {
        head.y+=30; //先把头退回来（不然结束游戏的时候头没了好丑哦）
        if(head.y>=600)
        {
            head.y=0;
        }
        EndGame();  //结束游戏
        return;
    }
    for(int i=BodyCount;i>=2;i--)   //每一节身体都向前移动到上一节身体位置
    {
        body[i]=body[i-1];
    }
    body[1]=head;   //第一节为头部位置
    direction=1;    //更新前进方向为上
    if(CheckEat())  //判断是否吃到了食物
    {
        EatFood();
    }
    if(CheckExEat())    //判断是否吃到了大仓鼠
    {
        EatExFood();
    }
    if(CheckItemEat())  //判断是否吃到了药丸
    {
        EatItemFood();
    }
    UpdateMap();    //刷新地图，标记蛇身位置
}

void MainWindow::DownMove() //向下操作
{
    if(direction==1)    //如果当前行进方向为上则为无效操作
    {
        return;
    }
    tail=body[BodyCount];   //更新尾部位置
    head.y+=30; //头向下移动一格
    if(head.y>=600) //穿墙
    {
        head.y=0;
    }
    if(CheckBodyCrush())    //检查头是否撞到了身体
    {
        head.y-=30; //头退回来
        if(head.y<0)
        {
            head.y=570;
        }
        EndGame();  //结束游戏
        return;
    }
    for(int i=BodyCount;i>=2;i--)   //身子往前移动
    {
        body[i]=body[i-1];
    }
    body[1]=head;   //第一格为头部位置
    direction=2;    //更新移动方向为下
    if(CheckEat())  //检查是否吃到了食物
    {
        EatFood();
    }
    if(CheckExEat())    //检查是否吃到了大仓鼠
    {
        EatExFood();
    }
    if(CheckItemEat())  //检查是否吃到了药丸
    {
        EatItemFood();
    }
    UpdateMap();    //更新地图标记蛇身位置
}

void MainWindow::LeftMove() //向左操作
{
    if(direction==4)    //如果当前方向为右则为无效操作
    {
        return;
    }
    tail=body[BodyCount];   //更新尾巴位置
    head.x-=30; //头向左移动一格
    if(head.x<0)    //穿墙
    {
        head.x=570;
    }
    if(CheckBodyCrush())    //检查头是否撞到了身体
    {
        head.x+=30; //头退回来
        if(head.x>=600)
        {
            head.x=0;
        }
        EndGame();  //结束游戏
        return;
    }
    for(int i=BodyCount;i>=2;i--)   //身体前移
    {
        body[i]=body[i-1];
    }
    body[1]=head;   //第一格为头部位置
    direction=3;    //当前移动方向置为左
    if(CheckEat())  //检查是否吃到了食物
    {
        EatFood();
    }
    if(CheckExEat())    //检查是否吃到了大仓鼠
    {
        EatExFood();
    }
    if(CheckItemEat())  //检查是否吃到了药丸
    {
        EatItemFood();
    }
    UpdateMap();    //更新游戏地图，标记蛇身位置
}

void MainWindow::RightMove()    //向右操作
{
    if(direction==3)    //如果当前方向为左则为无效操作
    {
        return;
    }
    tail=body[BodyCount];   //更新尾巴位置
    head.x+=30; //头部向右移动一格
    if(head.x>=600) //穿墙
    {
        head.x=0;
    }
    if(CheckBodyCrush())    //检查头是否撞到了身子
    {
        head.x-=30; //头退回来
        if(head.x<0)
        {
            head.x=570;
        }
        EndGame();  //结束游戏
        return;
    }
    for(int i=BodyCount;i>=2;i--)   //身体前移
    {
        body[i]=body[i-1];
    }
    body[1]=head;   //第一格为头部
    direction=4;    //移动方向置为向右
    if(CheckEat())  //检查是否吃到了食物
    {
        EatFood();
    }
    if(CheckExEat())    //检查是否吃到了大仓鼠
    {
        EatExFood();
    }
    if(CheckItemEat())  //检查是否吃到了药丸
    {
        EatItemFood();
    }
    UpdateMap();    //更新游戏地图，标记蛇身位置
}

void MainWindow::AutoMove() //无操作时自动移动，根据当前移动方向行走
{
    if(direction==1)
    {
        UpMove();
    }
    if(direction==2)
    {
        DownMove();
    }
    if(direction==3)
    {
        LeftMove();
    }
    if(direction==4)
    {
        RightMove();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *key)  //按键操作，判断移动方向
{
    if(key->key()==Qt::Key_Up)
    {
        UpMove();
    }
    if(key->key()==Qt::Key_Down)
    {
        DownMove();
    }
    if(key->key()==Qt::Key_Left)
    {
        LeftMove();
    }
    if(key->key()==Qt::Key_Right)
    {
        RightMove();
    }
    if(key->key()==Qt::Key_Escape)  //如果按了ESC键暂停游戏并询问是否退出
    {
        StopTimer();    //暂停游戏
        QMessageBox GameOverMsgBox;
        /*选是则退出，不然继续游戏*/
        if(GameOverMsgBox.Yes==GameOverMsgBox.question(this,tr("暂停游戏"),tr("<center>你当前得分是: %1 </center>\n <center>要退出游戏吗？</center>").arg(QString::number(score)),GameOverMsgBox.Yes|GameOverMsgBox.No,GameOverMsgBox.No))
        {
            QApplication::quit();
        }
        else
        {
            StartTimer();
            return;
        }
    }
    else
    {
        return;
    }
}

void MainWindow::SetLevel(int level)    //设置游戏等级，等级与移动速度挂钩
{
    if(level==1)
    {
        AutoMoveTimer->start(150);
        speedcontrol->setValue(30);
    }
    if(level==2)
    {
        AutoMoveTimer->start(120);
        speedcontrol->setValue(60);
    }
    if(level==3)
    {
        AutoMoveTimer->start(100);
        speedcontrol->setValue(80);
    }
    if(level==4)
    {
        AutoMoveTimer->start(80);
        speedcontrol->setValue(100);
    }
    if(level==5)
    {
        AutoMoveTimer->start(60);
        speedcontrol->setValue(120);
    }
    if(level>=6)
    {
        AutoMoveTimer->start(30);
        speedcontrol->setValue(150);
    }
}

void MainWindow::UpdateMap()    //更新游戏地图并标记蛇身位置
{
    for(int i=0;i<=20;i++)
        for(int j=0;j<=20;j++)
            map[i][j]=false;
    for(int i=1;i<=BodyCount;i++)
        map[body[i].x/30][body[i].y/30]=true;
}

void MainWindow::EndGame()  //结束游戏
{
    StopTimer();    //停止计时
    QSound::play(":/sound/gameover.wav");
    QMessageBox GameOverMsgBox;
    /*询问是否重新开始游戏，默认选是*/
    if(GameOverMsgBox.Yes==GameOverMsgBox.question(this,tr("Game Over!"),tr("<center>你的得分是: %1 </center>\n <center>点击“Yes”重新开始，点击“No”关闭游戏</center>").arg(QString::number(score)),GameOverMsgBox.Yes|GameOverMsgBox.No,GameOverMsgBox.Yes))
    {
        DeathTime++;
        ResetGame();
    }
    else
    {
        QApplication::quit();
    }
}

void MainWindow::ResetGame()    //重置游戏各项参数，便于重新开始游戏
{
    direction=4;

    level=1;
    score=0;

    ExistFood=false;
    ExistExFood=false;
    ExistItemFood=false;
    speedcontrol->setValue(30);

    CreateMap();
    CreateSnake();
    CreateFood();
    StartTimer();

}

void MainWindow::paintEvent(QPaintEvent *)  //绘图事件
{
    DrawScoreBoard();   //绘制计分板
    DrawScoreBoardTitile();//绘制记分板标题
    DrawBoard();    //绘制游戏背景
    DrawBody(); //绘制蛇身
    DrawFood(); //绘制食物
    if(level>=3&&ShowExFood())  //决定是否出现大仓鼠
    {
        DrawExFood();
    }
    else
    {
        ExistExFood=false;
    }
    if(level==4&&ShowItemFood()&&BodyCount>=5)  //决定是否出现药丸
    {
        DrawItemFood();
    }
    else
    {
        ExistItemFood=false;
    }
}

void MainWindow::setspeedcontrol()  //绘制调速转轮
{
    speedcontrol = new QDial(this);
    speedcontrol->setGeometry(660,446,90,90);
    speedcontrol->setRange(30,150);
    speedcontrol->setFocusPolicy(Qt::NoFocus);
    speedcontrol->show();
    connect(speedcontrol,SIGNAL(valueChanged(int)),this,SLOT(speedchange()));
}

void MainWindow::speedchange()  //调速转轮调速实现
{
    AutoMoveTimer->start(180-speedcontrol->value());
    if(speedcontrol->value()<60)
    {
        level=1;
    }
    else if(speedcontrol->value()<80)
    {
        level=2;
    }
    else if(speedcontrol->value()<100)
    {
        level=3;
    }
    else if(speedcontrol->value()<120)
    {
        level=4;
    }
    else if(speedcontrol->value()<150)
    {
        level=5;
    }
    else
    {
        level=6;
    }
}
