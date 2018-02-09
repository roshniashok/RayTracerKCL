/********************************************************************************
** Form generated from reading UI file 'raytracingg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAYTRACINGG_H
#define UI_RAYTRACINGG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayTracingg
{
public:
    QAction *actionSave_as_SVG;
    QAction *actionClear;
    QWidget *centralWidget;
    QComboBox *ShapeSelect;
    QLabel *Shapelabel;
    QLabel *Xlabel;
    QLabel *Ylabel;
    QSpinBox *XspinBox;
    QSpinBox *YspinBox;
    QLabel *label_4;
    QPushButton *SubmitButton;
    QPushButton *ClearButton;
    QToolButton *ColorButton;
    QLabel *label;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *RayTracingg)
    {
        if (RayTracingg->objectName().isEmpty())
            RayTracingg->setObjectName(QStringLiteral("RayTracingg"));
        RayTracingg->setEnabled(true);
        RayTracingg->resize(472, 480);
        actionSave_as_SVG = new QAction(RayTracingg);
        actionSave_as_SVG->setObjectName(QStringLiteral("actionSave_as_SVG"));
        actionClear = new QAction(RayTracingg);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        centralWidget = new QWidget(RayTracingg);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QStringLiteral(""));
        ShapeSelect = new QComboBox(centralWidget);
        ShapeSelect->setObjectName(QStringLiteral("ShapeSelect"));
        ShapeSelect->setGeometry(QRect(100, 280, 55, 20));
        Shapelabel = new QLabel(centralWidget);
        Shapelabel->setObjectName(QStringLiteral("Shapelabel"));
        Shapelabel->setGeometry(QRect(20, 280, 51, 21));
        Shapelabel->setStyleSheet(QLatin1String("Qlabel\n"
"{\n"
"color:white;\n"
"}"));
        Xlabel = new QLabel(centralWidget);
        Xlabel->setObjectName(QStringLiteral("Xlabel"));
        Xlabel->setGeometry(QRect(20, 310, 71, 16));
        Ylabel = new QLabel(centralWidget);
        Ylabel->setObjectName(QStringLiteral("Ylabel"));
        Ylabel->setGeometry(QRect(20, 340, 71, 16));
        XspinBox = new QSpinBox(centralWidget);
        XspinBox->setObjectName(QStringLiteral("XspinBox"));
        XspinBox->setGeometry(QRect(100, 310, 37, 20));
        XspinBox->setMaximum(10);
        YspinBox = new QSpinBox(centralWidget);
        YspinBox->setObjectName(QStringLiteral("YspinBox"));
        YspinBox->setGeometry(QRect(100, 340, 37, 20));
        YspinBox->setMaximum(10);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(21, 291, 16, 16));
        SubmitButton = new QPushButton(centralWidget);
        SubmitButton->setObjectName(QStringLiteral("SubmitButton"));
        SubmitButton->setGeometry(QRect(280, 420, 75, 23));
        ClearButton = new QPushButton(centralWidget);
        ClearButton->setObjectName(QStringLiteral("ClearButton"));
        ClearButton->setGeometry(QRect(370, 420, 75, 23));
        ColorButton = new QToolButton(centralWidget);
        ColorButton->setObjectName(QStringLiteral("ColorButton"));
        ColorButton->setGeometry(QRect(100, 370, 70, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 372, 47, 16));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(50, 30, 371, 221));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        RayTracingg->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RayTracingg);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 472, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        RayTracingg->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave_as_SVG);
        menuFile->addAction(actionClear);

        retranslateUi(RayTracingg);

        QMetaObject::connectSlotsByName(RayTracingg);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracingg)
    {
        RayTracingg->setWindowTitle(QApplication::translate("RayTracingg", "RayTracingg", 0));
        actionSave_as_SVG->setText(QApplication::translate("RayTracingg", "Save as SVG ", 0));
        actionClear->setText(QApplication::translate("RayTracingg", "Clear", 0));
        ShapeSelect->clear();
        ShapeSelect->insertItems(0, QStringList()
         << QApplication::translate("RayTracingg", "Select", 0)
         << QApplication::translate("RayTracingg", "Circle", 0)
         << QApplication::translate("RayTracingg", "Square", 0)
         << QApplication::translate("RayTracingg", "Triangle", 0)
         << QString()
        );
        Shapelabel->setText(QApplication::translate("RayTracingg", "Shape:", 0));
        Xlabel->setText(QApplication::translate("RayTracingg", "X co-ordinate:", 0));
        Ylabel->setText(QApplication::translate("RayTracingg", "Y co-ordinate:", 0));
        label_4->setText(QString());
        SubmitButton->setText(QApplication::translate("RayTracingg", "Submit", 0));
        ClearButton->setText(QApplication::translate("RayTracingg", "Clear", 0));
        ColorButton->setText(QApplication::translate("RayTracingg", "Select Color", 0));
        label->setText(QApplication::translate("RayTracingg", "Color:", 0));
        menuFile->setTitle(QApplication::translate("RayTracingg", "File ", 0));
    } // retranslateUi

};

namespace Ui {
    class RayTracingg: public Ui_RayTracingg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACINGG_H
