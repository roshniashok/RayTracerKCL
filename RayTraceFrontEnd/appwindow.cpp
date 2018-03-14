#include "appwindow.h"
#include "renderer.h"
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QPointF>
#include <QFile>
#include <QTextStream>
#include <QString>


Appwindow::Appwindow()
{
    drawingArea=new Renderer;
    //Select shape
    shapeComboBox=new QComboBox;
    shapeComboBox->addItem(tr("Circle"));
    shapeComboBox->addItem(tr("Square"));
    shapeComboBox->addItem(tr("Triangle"));

    shapeLabel=new QLabel(tr("&Shape:"));
    shapeLabel->setBuddy(shapeComboBox);

    //Select size
    sizeComboBox=new QComboBox;
    sizeComboBox->addItem(tr("Default"));
    sizeComboBox->addItem(tr("Scale to 50%"));
    sizeComboBox->addItem(tr("Scale to 75%"));
    sizeComboBox->addItem(tr("Scale to 90%"));

    sizeLabel=new QLabel(tr("&Size:"));
    sizeLabel->setBuddy(sizeComboBox);

    //Select Color
    colorComboBox = new QComboBox;
    FillWithColors(colorComboBox);
    colorComboBox->setCurrentIndex(colorComboBox->findText("red"));


    colorLabel=new QLabel(tr("&Color:"));
    colorLabel->setBuddy(colorComboBox);

    //Save to a file
    saveButton = new QPushButton(tr("&Save"));
    saveButton->setToolTip(tr("Save to a file for Raytracing"));
    saveButton->setEnabled(true);

    //Close program
    closeButton = new QPushButton(tr("&Close"));

    closeButton->setEnabled(true);



    //Signals and Slots

    connect(sizeComboBox,SIGNAL(activated(int)),this,SLOT(sizeChanged()));
    connect(colorComboBox,SIGNAL(activated(int)),this,SLOT(colorChanged()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeWindow()));


    //GridLayout for widgets

    QGridLayout *mainLayout=new QGridLayout;

    mainLayout->addWidget(drawingArea, 0, 0, 1, 4);
    mainLayout->addWidget(shapeLabel,1,0);
    mainLayout->addWidget(shapeComboBox, 1,1,1,3);
    mainLayout->addWidget(sizeLabel, 2,0);
    mainLayout->addWidget(sizeComboBox, 2,1,1,3);
   mainLayout->addWidget(colorLabel, 3,0);
   mainLayout->addWidget(colorComboBox, 3,1,1,3);
   mainLayout->addWidget(saveButton,5,1,Qt::AlignRight);
   mainLayout->addWidget(closeButton,5,2,Qt::AlignLeft);

    setLayout(mainLayout);
    drawShapes();
    shapeSelected(0);
    sizeChanged();
    colorChanged();
    closeWindow();


    setWindowTitle(tr("2D Shapes:Input for RayTracer"));

}

void Appwindow::drawShapes()
{
    QPainterPath Circle;
    Circle.addEllipse(0.0,0.0,250.0,250.0);

    QPainterPath Square;
    Square.addRect(0.0,0.0,250.0,250.0);

    QPainterPath Triangle;
    QPolygonF Polytri;
    Polytri<<QPointF(200.0,100.0)<<QPointF(100.0,300.0)<<QPointF(300.0,300.0);

    Triangle.addPolygon(Polytri);

    shapes.append(Circle);
    shapes.append(Square);
    shapes.append(Triangle);

    connect(shapeComboBox, SIGNAL(activated(int)), this, SLOT(shapeSelected(int)));

}

void Appwindow::shapeSelected(int index)
{
    QPainterPath shape = shapes[index];
    drawingArea->setShape(shape);

}

void Appwindow::sizeChanged()
{
  static const ShapeSize operationTable[]={
      S25,S50,S75,S90
  };

  QList<ShapeSize> ShapeSizes;
  int index=sizeComboBox->currentIndex();
  ShapeSizes.append(operationTable[index]);
  drawingArea->setSize(ShapeSizes);
}

void Appwindow::colorChanged()
{
    QColor color = qvariant_cast<QColor>(currentItemData(colorComboBox));
    drawingArea->setColor(color);
}

void Appwindow::FillWithColors(QComboBox *comboBox)
{
    QStringList colorNames = QColor::colorNames();
    foreach (QString name, colorNames)
        comboBox->addItem(name, QColor(name));
}

QVariant Appwindow::currentItemData(QComboBox *comboBox)
{
    return comboBox->itemData(comboBox->currentIndex());
}

void Appwindow::saveToFile()
{
    QString filename="Scene.txt";
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text))
    {
        QTextStream out(&file);
        out<<shapeComboBox->currentText()<<"\r\n";
        out<<sizeComboBox->currentText()<<"\r\n";
        out<<colorComboBox->currentText()<<"\r\n";
        out<<"\r\n";
    }

}

void Appwindow::closeWindow()
{
   this->close();
}

