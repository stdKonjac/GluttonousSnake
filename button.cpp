#include "button.h"
Button::Button(QWidget *parent) : QPushButton(parent)
{
    //保存图片成员初始化
    buttonPicture = new QPixmap();
    pressPicture = new QPixmap();
    releasePicture = new QPixmap();

    //关闭按钮的默认显示
    this -> setFlat(true);
}

Button::~Button()
{

}

void Button::setButtonPicture(QPixmap pic)
{
    *buttonPicture = pic;

    this -> setIcon(QIcon(*buttonPicture));
}

void Button::setPressPicture(QPixmap pic)
{
    *pressPicture = pic;
}

void Button::setReleasePicture(QPixmap pic)
{
    *releasePicture	= pic;
}

void Button::set_Size(int width, int height)
{
    this -> setIconSize(QSize(width, height));
}

void Button::mousePressEvent (QMouseEvent *event)
{
    this -> setIcon (QIcon(*pressPicture));
}

void Button::mouseReleaseEvent (QMouseEvent *event)
{
    this -> setIcon(QIcon(*releasePicture));
    emit clicked();
}

void Button::enterEvent(QEvent *event)
{

    this ->setIconSize(QSize(180, 120));
    this ->setIcon(QIcon(*buttonPicture));
}

void Button::leaveEvent(QEvent *event)
{
    this -> setIconSize(QSize(120, 80));
    this -> setIcon(QIcon(*buttonPicture));
}
