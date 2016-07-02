/********************************************************************************
** Form generated from reading UI file 'ImageGenerationDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEGENERATIONDIALOG_H
#define UI_IMAGEGENERATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ImageGenerationDialog
{
public:
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;
    QComboBox *comboBoxObjectType;
    QLabel *label;
    QLineEdit *lineEditCGADirectory;
    QLabel *label_2;
    QPushButton *pushButtonCGADirectory;
    QLabel *label_3;
    QLineEdit *lineEditOutputDirectory;
    QPushButton *pushButtonOutputDirectory;
    QLabel *label_4;
    QCheckBox *checkBoxGrayscale;
    QLabel *label_5;
    QLineEdit *lineEditImageSize;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEditEdgeDisplacement;
    QLabel *label_8;
    QCheckBox *checkBoxRemoveInvalid;
    QCheckBox *checkBoxSketchyLine;

    void setupUi(QDialog *ImageGenerationDialog)
    {
        if (ImageGenerationDialog->objectName().isEmpty())
            ImageGenerationDialog->setObjectName(QStringLiteral("ImageGenerationDialog"));
        ImageGenerationDialog->resize(542, 255);
        pushButtonOK = new QPushButton(ImageGenerationDialog);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(320, 210, 91, 31));
        pushButtonCancel = new QPushButton(ImageGenerationDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(430, 210, 91, 31));
        comboBoxObjectType = new QComboBox(ImageGenerationDialog);
        comboBoxObjectType->setObjectName(QStringLiteral("comboBoxObjectType"));
        comboBoxObjectType->setGeometry(QRect(120, 20, 151, 22));
        label = new QLabel(ImageGenerationDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 61, 21));
        lineEditCGADirectory = new QLineEdit(ImageGenerationDialog);
        lineEditCGADirectory->setObjectName(QStringLiteral("lineEditCGADirectory"));
        lineEditCGADirectory->setGeometry(QRect(120, 50, 371, 20));
        label_2 = new QLabel(ImageGenerationDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 50, 81, 21));
        pushButtonCGADirectory = new QPushButton(ImageGenerationDialog);
        pushButtonCGADirectory->setObjectName(QStringLiteral("pushButtonCGADirectory"));
        pushButtonCGADirectory->setGeometry(QRect(500, 50, 31, 21));
        label_3 = new QLabel(ImageGenerationDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 80, 91, 21));
        lineEditOutputDirectory = new QLineEdit(ImageGenerationDialog);
        lineEditOutputDirectory->setObjectName(QStringLiteral("lineEditOutputDirectory"));
        lineEditOutputDirectory->setGeometry(QRect(120, 80, 371, 20));
        pushButtonOutputDirectory = new QPushButton(ImageGenerationDialog);
        pushButtonOutputDirectory->setObjectName(QStringLiteral("pushButtonOutputDirectory"));
        pushButtonOutputDirectory->setGeometry(QRect(500, 80, 31, 21));
        label_4 = new QLabel(ImageGenerationDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 110, 81, 21));
        checkBoxGrayscale = new QCheckBox(ImageGenerationDialog);
        checkBoxGrayscale->setObjectName(QStringLiteral("checkBoxGrayscale"));
        checkBoxGrayscale->setGeometry(QRect(290, 110, 70, 21));
        label_5 = new QLabel(ImageGenerationDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(240, 110, 41, 21));
        lineEditImageSize = new QLineEdit(ImageGenerationDialog);
        lineEditImageSize->setObjectName(QStringLiteral("lineEditImageSize"));
        lineEditImageSize->setGeometry(QRect(120, 110, 51, 20));
        label_6 = new QLabel(ImageGenerationDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(180, 110, 21, 21));
        label_7 = new QLabel(ImageGenerationDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 140, 101, 16));
        lineEditEdgeDisplacement = new QLineEdit(ImageGenerationDialog);
        lineEditEdgeDisplacement->setObjectName(QStringLiteral("lineEditEdgeDisplacement"));
        lineEditEdgeDisplacement->setGeometry(QRect(120, 140, 51, 20));
        label_8 = new QLabel(ImageGenerationDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(180, 140, 21, 21));
        checkBoxRemoveInvalid = new QCheckBox(ImageGenerationDialog);
        checkBoxRemoveInvalid->setObjectName(QStringLiteral("checkBoxRemoveInvalid"));
        checkBoxRemoveInvalid->setGeometry(QRect(380, 140, 131, 21));
        checkBoxSketchyLine = new QCheckBox(ImageGenerationDialog);
        checkBoxSketchyLine->setObjectName(QStringLiteral("checkBoxSketchyLine"));
        checkBoxSketchyLine->setGeometry(QRect(240, 140, 111, 21));

        retranslateUi(ImageGenerationDialog);

        QMetaObject::connectSlotsByName(ImageGenerationDialog);
    } // setupUi

    void retranslateUi(QDialog *ImageGenerationDialog)
    {
        ImageGenerationDialog->setWindowTitle(QApplication::translate("ImageGenerationDialog", "ImageGenerationDialog", 0));
        pushButtonOK->setText(QApplication::translate("ImageGenerationDialog", "OK", 0));
        pushButtonCancel->setText(QApplication::translate("ImageGenerationDialog", "Cancel", 0));
        label->setText(QApplication::translate("ImageGenerationDialog", "Object type:", 0));
        label_2->setText(QApplication::translate("ImageGenerationDialog", "CGA directory:", 0));
        pushButtonCGADirectory->setText(QApplication::translate("ImageGenerationDialog", "...", 0));
        label_3->setText(QApplication::translate("ImageGenerationDialog", "Output directory:", 0));
        pushButtonOutputDirectory->setText(QApplication::translate("ImageGenerationDialog", "...", 0));
        label_4->setText(QApplication::translate("ImageGenerationDialog", "Image size:", 0));
        checkBoxGrayscale->setText(QApplication::translate("ImageGenerationDialog", "Grayscale", 0));
        label_5->setText(QApplication::translate("ImageGenerationDialog", "Color:", 0));
        label_6->setText(QApplication::translate("ImageGenerationDialog", "px", 0));
        label_7->setText(QApplication::translate("ImageGenerationDialog", "Edge displacement:", 0));
        label_8->setText(QApplication::translate("ImageGenerationDialog", "px", 0));
        checkBoxRemoveInvalid->setText(QApplication::translate("ImageGenerationDialog", "Remove invalid images", 0));
        checkBoxSketchyLine->setText(QApplication::translate("ImageGenerationDialog", "Use sketchy lines", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageGenerationDialog: public Ui_ImageGenerationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEGENERATIONDIALOG_H
