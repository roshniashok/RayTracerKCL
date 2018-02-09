#ifndef RAYTRACINGG_H
#define RAYTRACINGG_H

#include <QMainWindow>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>

namespace Ui {
class RayTracingg;
}

class RayTracingg : public QMainWindow
{
    Q_OBJECT

public:
    explicit RayTracingg(QWidget *parent = 0);
    ~RayTracingg();
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void on_SubmitButton_clicked();
    void on_ColorButton_clicked();


private:
    Ui::RayTracingg *ui;
    void doShapes();
};

#endif // RAYTRACINGG_H
