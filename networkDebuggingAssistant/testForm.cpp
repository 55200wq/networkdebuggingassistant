#include "testForm.h"
#include "ui_testForm.h"
#include <QPainter>

testForm::testForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testForm)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));


}

testForm::~testForm()
{
    delete ui;
}

void testForm::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.setRenderHint();
    int width, height;
    width = testForm::width();
    height = testForm::height();
    QRect rect(width/4, height/4, width/2, height/2);//左上角坐标和矩形中间的坐标
    //QRect rect(width/4, height/4);

    //设置画笔
    QPen pen;
    pen.setWidth(5);//设置线宽
    pen.setColor(Qt::blue);//设置线的颜色
    pen.setStyle(Qt::SolidLine);//线的样式,实线、虚线等
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    pen.setCapStyle(Qt::FlatCap);//端点样式
    painter.setPen(pen);

    //设置画刷
    QBrush brush;
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::SolidPattern);//设置画刷填充样式
    //painter.setBrush(brush);
    painter.drawRect(rect);//画矩形

    //加载图片
    QImage img("C:/Users/lenovo/Desktop/圆检测判断条件.PNG");
    QPixmap pix("C:/Users/lenovo/Desktop/直线检测判断条件.PNG");
    //painter.drawImage(0,0, img);
    //painter.drawPixmap(0, 0, pix);
}
