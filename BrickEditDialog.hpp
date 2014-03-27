#ifndef BRICKEDITDIALOG_HPP
#define BRICKEDITDIALOG_HPP

#include <QDialog>
#include <QString>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QSpinBox>

#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"

#include <iostream>

typedef ScriptBrickIf::ComparisionOperators ComparisionOperator;



struct BrickBasicData
{
    QString left, right;
    ComparisionOperator op;
    StatusT status;
};


class BrickEditDialog : public QDialog
{
    Q_OBJECT

    BrickBasicData data;

    QGridLayout *layout;

    QLabel *labelLeft, *labelOperator, *labelRight, *labelColor;
    QComboBox *comboOperator;
    QSpinBox *selectColor;
    QLineEdit *editLeft, *editRight;
    QPushButton *buttonCancel, *buttonApply;

    void showEvent(QShowEvent *e);

public:
    explicit BrickEditDialog(QWidget *parent);

    void setData(BrickBasicData d);
    BrickBasicData getData();

private slots:
    void cancel();
    void apply();

};

#endif // BRICKEDITDIALOG_HPP
