/********************************************************************************
** Form generated from reading UI file 'ParameterEstimationDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERESTIMATIONDIALOG_H
#define UI_PARAMETERESTIMATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ParameterEstimationDialog
{
public:
    QLineEdit *lineEditTestDataDirectory;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonOK;
    QLabel *label_12;
    QLabel *label_15;
    QPushButton *pushButtonTestDataDirectory;
    QLineEdit *lineEditClassificationDirectory;
    QLabel *label_16;
    QLineEdit *lineEditRegressionDirectory;
    QPushButton *pushButtonClassificationDirectory;
    QPushButton *pushButtonRegressionDirectory;
    QLabel *label_17;
    QLineEdit *lineEditOutputDirectory;
    QPushButton *pushButtonOutputDirectory;
    QLabel *label_22;
    QComboBox *comboBoxObjectType;
    QLabel *label_18;
    QLineEdit *lineEditCGADirectory;
    QPushButton *pushButtonCGADirectory;

    void setupUi(QDialog *ParameterEstimationDialog)
    {
        if (ParameterEstimationDialog->objectName().isEmpty())
            ParameterEstimationDialog->setObjectName(QStringLiteral("ParameterEstimationDialog"));
        ParameterEstimationDialog->resize(526, 255);
        lineEditTestDataDirectory = new QLineEdit(ParameterEstimationDialog);
        lineEditTestDataDirectory->setObjectName(QStringLiteral("lineEditTestDataDirectory"));
        lineEditTestDataDirectory->setGeometry(QRect(140, 80, 331, 20));
        pushButtonCancel = new QPushButton(ParameterEstimationDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(414, 210, 91, 31));
        pushButtonOK = new QPushButton(ParameterEstimationDialog);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(314, 210, 91, 31));
        label_12 = new QLabel(ParameterEstimationDialog);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(20, 110, 121, 20));
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_15 = new QLabel(ParameterEstimationDialog);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(20, 80, 101, 20));
        label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pushButtonTestDataDirectory = new QPushButton(ParameterEstimationDialog);
        pushButtonTestDataDirectory->setObjectName(QStringLiteral("pushButtonTestDataDirectory"));
        pushButtonTestDataDirectory->setGeometry(QRect(480, 80, 31, 23));
        lineEditClassificationDirectory = new QLineEdit(ParameterEstimationDialog);
        lineEditClassificationDirectory->setObjectName(QStringLiteral("lineEditClassificationDirectory"));
        lineEditClassificationDirectory->setGeometry(QRect(140, 110, 331, 20));
        label_16 = new QLabel(ParameterEstimationDialog);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(20, 140, 121, 20));
        lineEditRegressionDirectory = new QLineEdit(ParameterEstimationDialog);
        lineEditRegressionDirectory->setObjectName(QStringLiteral("lineEditRegressionDirectory"));
        lineEditRegressionDirectory->setGeometry(QRect(140, 140, 331, 20));
        pushButtonClassificationDirectory = new QPushButton(ParameterEstimationDialog);
        pushButtonClassificationDirectory->setObjectName(QStringLiteral("pushButtonClassificationDirectory"));
        pushButtonClassificationDirectory->setGeometry(QRect(480, 110, 31, 23));
        pushButtonRegressionDirectory = new QPushButton(ParameterEstimationDialog);
        pushButtonRegressionDirectory->setObjectName(QStringLiteral("pushButtonRegressionDirectory"));
        pushButtonRegressionDirectory->setGeometry(QRect(480, 140, 31, 23));
        label_17 = new QLabel(ParameterEstimationDialog);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(20, 170, 121, 20));
        lineEditOutputDirectory = new QLineEdit(ParameterEstimationDialog);
        lineEditOutputDirectory->setObjectName(QStringLiteral("lineEditOutputDirectory"));
        lineEditOutputDirectory->setGeometry(QRect(140, 170, 331, 20));
        pushButtonOutputDirectory = new QPushButton(ParameterEstimationDialog);
        pushButtonOutputDirectory->setObjectName(QStringLiteral("pushButtonOutputDirectory"));
        pushButtonOutputDirectory->setGeometry(QRect(480, 170, 31, 23));
        label_22 = new QLabel(ParameterEstimationDialog);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(20, 20, 101, 20));
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        comboBoxObjectType = new QComboBox(ParameterEstimationDialog);
        comboBoxObjectType->setObjectName(QStringLiteral("comboBoxObjectType"));
        comboBoxObjectType->setGeometry(QRect(140, 20, 161, 22));
        label_18 = new QLabel(ParameterEstimationDialog);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(20, 50, 101, 20));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEditCGADirectory = new QLineEdit(ParameterEstimationDialog);
        lineEditCGADirectory->setObjectName(QStringLiteral("lineEditCGADirectory"));
        lineEditCGADirectory->setGeometry(QRect(140, 50, 331, 20));
        pushButtonCGADirectory = new QPushButton(ParameterEstimationDialog);
        pushButtonCGADirectory->setObjectName(QStringLiteral("pushButtonCGADirectory"));
        pushButtonCGADirectory->setGeometry(QRect(480, 50, 31, 23));
        QWidget::setTabOrder(lineEditTestDataDirectory, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButtonCancel);

        retranslateUi(ParameterEstimationDialog);

        QMetaObject::connectSlotsByName(ParameterEstimationDialog);
    } // setupUi

    void retranslateUi(QDialog *ParameterEstimationDialog)
    {
        ParameterEstimationDialog->setWindowTitle(QApplication::translate("ParameterEstimationDialog", "ParameterEstimationDialog", 0));
        pushButtonCancel->setText(QApplication::translate("ParameterEstimationDialog", "Cancel", 0));
        pushButtonOK->setText(QApplication::translate("ParameterEstimationDialog", "OK", 0));
        label_12->setText(QApplication::translate("ParameterEstimationDialog", "Classification directory:", 0));
        label_15->setText(QApplication::translate("ParameterEstimationDialog", "Test data directory:", 0));
        pushButtonTestDataDirectory->setText(QApplication::translate("ParameterEstimationDialog", "...", 0));
        label_16->setText(QApplication::translate("ParameterEstimationDialog", "Regression directory:", 0));
        pushButtonClassificationDirectory->setText(QApplication::translate("ParameterEstimationDialog", "...", 0));
        pushButtonRegressionDirectory->setText(QApplication::translate("ParameterEstimationDialog", "...", 0));
        label_17->setText(QApplication::translate("ParameterEstimationDialog", "Output directory:", 0));
        pushButtonOutputDirectory->setText(QApplication::translate("ParameterEstimationDialog", "...", 0));
        label_22->setText(QApplication::translate("ParameterEstimationDialog", "Object type:", 0));
        label_18->setText(QApplication::translate("ParameterEstimationDialog", "CGA directory:", 0));
        pushButtonCGADirectory->setText(QApplication::translate("ParameterEstimationDialog", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class ParameterEstimationDialog: public Ui_ParameterEstimationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERESTIMATIONDIALOG_H
