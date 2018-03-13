#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "renderer.h"

#include <QWidget>
#include <QPainterPath>
#include <QList>
#include <QPolygonF>

class QLabel;
class QComboBox;
class QPushButton;

class Renderer;

class Appwindow:public QWidget
{
    Q_OBJECT

public:
    Appwindow();

public slots:
    void shapeSelected(int index);
    void sizeChanged();
    void colorChanged();
    void saveToFile();
    void closeWindow();

private:

    void drawShapes();
    void FillWithColors(QComboBox *comboBox);

    QVariant currentItemData(QComboBox *comboBox);
    Renderer *drawingArea;
    QLabel *shapeLabel;
    QLabel *sizeLabel;
    QLabel *colorLabel;
    QPushButton *saveButton;
    QPushButton *closeButton;

    QComboBox *shapeComboBox;
    QComboBox *sizeComboBox;
    QComboBox *colorComboBox;
    QList<QPainterPath> shapes;
};



#endif // APPWINDOW_H
