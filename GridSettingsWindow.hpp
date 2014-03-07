#ifndef GRIDSETTINGDWINDOW_HPP
#define GRIDSETTINGDWINDOW_HPP

#include <QMainWindow>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QFormLayout>
#include <iostream>

class MainWindow;



class GridSettingsWindow : public QDialog
{
    Q_OBJECT
private:
    MainWindow *parent;

    QGridLayout *form;

    QSpinBox *widthBox, *heightBox;
    QGroupBox *sizeForm;
    QFormLayout *sizeFormLayout;
    void createSizeSettings(int row);

    QPushButton *applyButton, *cancelButton;
    void createButtons(int row);

    void showEvent(QShowEvent *e);

public:
    GridSettingsWindow(MainWindow *parent);

public slots:
    void applySettings();
    void readSettings();

};

#endif // GRIDSETTINGDWINDOW_HPP
