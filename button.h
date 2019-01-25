#ifndef BUTTON_H
#define BUTTON_H
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>
#include <QSize>

class Button:public QPushButton
{
    Q_OBJECT
public:
    Button(QWidget *parent=0);
    ~Button();
    void setButtonPicture(QPixmap pic);
    void setPressPicture(QPixmap pic);
    void setReleasePicture(QPixmap pic);
    void set_Size(int width, int height);
    void mousePressEvent (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private:
    QPixmap  *buttonPicture;
    QPixmap  *pressPicture;
    QPixmap  *releasePicture;
};
#endif // BUTTON_H
