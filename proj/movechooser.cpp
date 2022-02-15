#include "movechooser.hpp"
#include "ui_movechooser.h"

#include <QErrorMessage>

moveChooser::moveChooser(QWidget *parent) : QDialog(parent), ui(new Ui::moveChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Изменение положения модели на сцене");
}

moveChooser::~moveChooser()
{
    delete ui;
}

int moveChooser::getNewXCell()
{
    return newXCell;
}

int moveChooser::getNewYCell()
{
    return newYCell;
}


void moveChooser::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit_4->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    newXCell = gotText.toInt();

    gotText = this->ui->lineEdit_3->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    newYCell = gotText.toInt();

    if (newXCell <= 0 || newYCell <= 0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Ошибка. Неверно введены данные.\n"
                         "Номера ячеек - целые положительные числа.");
        return;
    }
    else
        status = OK;

    this->close();
}

void moveChooser::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}
