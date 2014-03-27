#include "BrickEditDialog.hpp"


BrickEditDialog::BrickEditDialog(QWidget *parent) :
    QDialog(parent)
{
    layout = new QGridLayout(this);

    labelLeft = new QLabel(tr("Lewe wyrażenie"), this);
    labelOperator = new QLabel(tr("Operator porównania"), this);
    labelRight = new QLabel(tr("Prawe wyrażenie"), this);
    labelColor = new QLabel(tr("Kolor bloku"), this);

    editLeft = new QLineEdit(this);
    editRight = new QLineEdit(this);
    comboOperator = new QComboBox(this);
    selectColor = new QSpinBox(this);

    buttonCancel = new QPushButton(tr("Anuluj"), this);
    buttonApply = new QPushButton(tr("OK"), this);

    layout->addWidget(labelLeft, 0, 0);
    layout->addWidget(labelOperator, 1, 0);
    layout->addWidget(labelRight, 2, 0);
    layout->addWidget(labelColor, 3, 0);
    layout->addWidget(buttonCancel, 4, 0);
    layout->addWidget(editLeft, 0, 1);
    layout->addWidget(comboOperator, 1, 1);
    layout->addWidget(editRight, 2, 1);
    layout->addWidget(selectColor, 3, 1);
    layout->addWidget(buttonApply, 4, 1);

    selectColor->setMinimum(0);
    selectColor->setMaximum(STATUS_NUMBER - 1);

    comboOperator->addItem("= (równe)", ScriptBrickIf::EQUAL);
    comboOperator->addItem("≠ (różne)", ScriptBrickIf::NOT_EQUAL);
    comboOperator->addItem("< (mniejsze)", ScriptBrickIf::LESS);
    comboOperator->addItem("≤ (niewiększe)", ScriptBrickIf::LESS_OR_EQUAL);
    comboOperator->addItem("> (większe)", ScriptBrickIf::GREATER);
    comboOperator->addItem("≥ (niemniejsze)", ScriptBrickIf::GREATER_OR_EQUAL);

    setModal(true);

    connect(buttonApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
}


void BrickEditDialog::setData(BrickBasicData d)
{
    data = d;
}


BrickBasicData BrickEditDialog::getData()
{
    return data;
}


void BrickEditDialog::cancel()
{
    reject();
}


void BrickEditDialog::apply()
{
    QMessageBox syntaxErrorMessageBox(this);
    syntaxErrorMessageBox.setIcon(QMessageBox::Critical);
    syntaxErrorMessageBox.setWindowTitle(tr("Błąd składni wyrażenia"));

    SyntaxCheckResult chk;
    chk = CellularAutomaton::checkSyntaxOfMathexpr(editLeft->text().toUtf8().constData());
    if (chk.msg != "OK") {
        syntaxErrorMessageBox.setText(tr("Błąd składnii w prawym wyrażeniu:") + chk.msg);
        syntaxErrorMessageBox.exec();
        return;
    }

    chk = CellularAutomaton::checkSyntaxOfMathexpr(editRight->text().toUtf8().constData());
    if (chk.msg != "OK") {
        syntaxErrorMessageBox.setText(tr("Błąd składnii w prawym wyrażeniu:") + chk.msg);
        syntaxErrorMessageBox.exec();
        return;
    }

    data.left = editLeft->text();
    data.right = editRight->text();

    switch (comboOperator->currentData().toInt()) {
    case ComparisionOperator::EQUAL :
        data.op = ComparisionOperator::EQUAL;
        break;
    case ComparisionOperator::NOT_EQUAL :
        data.op = ComparisionOperator::NOT_EQUAL;
        break;
    case ComparisionOperator::GREATER :
        data.op = ComparisionOperator::GREATER;
        break;
    case ComparisionOperator::GREATER_OR_EQUAL :
        data.op = ComparisionOperator::GREATER_OR_EQUAL;
        break;
    case ComparisionOperator::LESS :
        data.op = ComparisionOperator::LESS;
        break;
    case ComparisionOperator::LESS_OR_EQUAL :
        data.op = ComparisionOperator::LESS_OR_EQUAL;
        break;
    }

    data.status = selectColor->value();

    accept();
}


void BrickEditDialog::showEvent(QShowEvent *e)
{
    comboOperator->setCurrentIndex(comboOperator->findData(data.op));
    selectColor->setValue(data.status);
    editLeft->setText(data.left);
    editRight->setText(data.right);
}
