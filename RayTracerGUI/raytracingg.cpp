#include "raytracingg.h"
#include "ui_raytracingg.h"
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>
#include <QPainter>
#include <QColor>


RayTracingg::RayTracingg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RayTracingg)
{
    ui->setupUi(this);
    QObject::connect(ui->SubmitButton,SIGNAL(clicked()),this,SLOT(doShapes()));
}

void RayTracingg::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    doShapes();
}

void RayTracingg::doShapes()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor backgroundColor = palette().light().color();
    backgroundColor.setAlpha(200);
    painter.setPen(QColor("#000000"));
    painter.setBrush(QBrush(QColor("#FFFFFF")));
    painter.drawRect(130, 105, 90, 90);
    painter.drawEllipse(240, 200, 60, 60);

}

RayTracingg::~RayTracingg()
{
    delete ui;
}

/*void RayTracingg::setShape(Shape shape)
{
    this->shape = shape;
    update();
}*/

void RayTracingg::on_SubmitButton_clicked()
{
  //QMessageBox::information(this,"Incomplete information","Please enter all fields");

  }



void RayTracingg::on_ColorButton_clicked()
{
    QColor color;
    color=QColorDialog::getColor(Qt::white,this);

}
