#include "renderer.h"
#include <QPainter>
#include <QPaintEvent>

Renderer::Renderer(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Renderer::setShape(const QPainterPath &shape)
{
    this->shape=shape;
    update();
}

void Renderer::setSize(const QList<ShapeSize> &ShapeSizes)
{
    this->ShapeSizes=ShapeSizes;
    update();
}

void Renderer::setColor(const QColor &color)
{
    ShapeColor = color;
    update();
}

QSize Renderer::minimumSizeHint() const
{
    return QSize(250, 250);
}

QSize Renderer::sizeHint() const
{
    return QSize(500, 350);
}

void Renderer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(150,50);

       painter.save();
       transformPainter(painter);
       createShape(painter);
       painter.restore();

      // drawOutline(painter);

       transformPainter(painter);
       fillShapeColor(painter);


}


void Renderer::createShape(QPainter &painter)
{
    painter.fillPath(shape, Qt::white);

}

void Renderer::fillShapeColor(QPainter &painter)
{
    painter.fillPath(shape,ShapeColor);
}

void Renderer::transformPainter(QPainter &painter)
{
    for (int i = 0; i < ShapeSizes.size(); ++i) {
        switch (ShapeSizes[i]) {
        case S25:
            painter.scale(0.25, 0.25);
            break;
        case S50:
            painter.scale(0.50, 0.50);
            break;
        case S75:
            painter.scale(0.75,0.75);
            break;
        case S90:
        default:
            painter.scale(0.90,0.90);
            ;
        }
    }
}

