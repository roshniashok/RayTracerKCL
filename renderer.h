#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QList>
#include <QPolygonF>
#include <QPainterPath>

class QPaintEvent;

enum  ShapeSize {S25,S50,S75,S90};

class Renderer : public QWidget
{
    Q_OBJECT

public:
    Renderer(QWidget *parent = 0);
    void setSize(const QList<ShapeSize> &ShapeSizes);
    void setShape(const QPainterPath &shape);
    void setColor(const QColor &color);


    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void createShape(QPainter &painter);
    //void drawOutline(QPainter &painter);
    void transformPainter(QPainter &painter);
    void fillShapeColor(QPainter &painter);

    QColor ShapeColor;
    QList<ShapeSize> ShapeSizes;
    QPainterPath shape;

};

#endif // RENDERER_H
