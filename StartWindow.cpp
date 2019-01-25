#include "StartWindow.h"

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent)
{
    setStartButton();   //设定开始按钮
    setExitButton();    //设定退出按钮
    setGameTitle(); //设定游戏标题
}

StartWindow::~StartWindow()
{

}

QPoint StartWindow::GetScreenCenter()   //获取屏幕中心
{
    int X=(QApplication::desktop()->width() - this->width())/2;
    int Y=(QApplication::desktop()->height() - this->height())/2;
    QPoint ScreenCenter;
    ScreenCenter.setX(X);
    ScreenCenter.setY(Y);
    return ScreenCenter;
}

void StartWindow::setStartButton()  //设定开始按钮
{
    StartButton=new Button(this);
    QPixmap image1(":/image/images/kaishi.png");
    QPixmap image2(":/image/images/image_emoticon25.png");
    StartButton->setPressPicture(image2);
    StartButton->setButtonPicture(image1);
    StartButton->setReleasePicture(image1);
    StartButton->setGeometry(70,280,210,140);
    StartButton->set_Size(120,80);
    StartButton->show();
    /*点击开始，关闭开始界面窗口并发出开始信号*/
    connect(StartButton,&QPushButton::clicked,this,&StartWindow::close);
    connect(StartButton,&QPushButton::clicked,this,&StartWindow::SendStartSignal);
}

void StartWindow::setExitButton()   //设定结束按钮
{
    ExitButton=new Button(this);
    QPixmap image1(":/image/images/jieshu.png");
    QPixmap image2(":/image/images/image_emoticon25.png");
    ExitButton->setPressPicture(image2);
    ExitButton->setButtonPicture(image1);
    ExitButton->setReleasePicture(image1);
    ExitButton->setGeometry(460,280,210,140);
    ExitButton->set_Size(120,80);
    ExitButton->show();
    connect(ExitButton,&QPushButton::clicked,this,&QApplication::quit); //点击结束则退出游戏
}

void StartWindow::setGameTitle()    //设定游戏标题
{
    GameTitle=new QLabel(this);
    GameTitle->setText("贪吃蛇");
    GameTitle->setGeometry(230,100,350,100);
    QFont TitleFont;
    TitleFont.setFamily("方正字迹-杜慧田硬笔楷书");
    TitleFont.setPointSize(55);
    GameTitle->setFont(TitleFont);
    QPalette TitlePalette;
    TitlePalette.setColor(QPalette::WindowText,Qt::gray);
    GameTitle->setPalette(TitlePalette);
    GameTitle->show();

}

void StartWindow::paintEvent(QPaintEvent *) //绘制开始界面
{
    QPainter painter(this);
    QPixmap BKPic(":/image/startBG.jpeg");
    painter.drawPixmap(0,0,750,460,BKPic);
}

void StartWindow::SendStartSignal() //发出开始信号
{
    emit StartSignal();
}
