#include "illuminantplacechooser.hpp"
#include "ui_illuminantplacechooser.h"

#include <QErrorMessage>

IlluminantPlaceChooser::IlluminantPlaceChooser(QWidget *parent) : QDialog(parent), ui(new Ui::IlluminantPlaceChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор расположения источника света");
}

IlluminantPlaceChooser::~IlluminantPlaceChooser()
{
    delete ui;
}

void IlluminantPlaceChooser::setXAngle(int degrees)
{
    xAngle = degrees;
}

int IlluminantPlaceChooser::getXAngle()
{
    return xAngle;
}

void IlluminantPlaceChooser::setYAngle(int degrees)
{
    yAngle = degrees;
}

int IlluminantPlaceChooser::getYAngle()
{
    return yAngle;
}

void IlluminantPlaceChooser::on_buttonBox_accepted()
{
    status = CANCEL;
    bool xAngleOk, yAngleOk;

    QString gotText = this->ui->XAngleInp->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xAngle = gotText.toInt(&xAngleOk);

    gotText = this->ui->YAngleInp->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yAngle = gotText.toInt(&yAngleOk);

    if (!xAngleOk || !yAngleOk)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Ошибка. Неверно введены данные.\n"
                         "Ожидался ввод целых чисел");
        return;
    }
    else
        status = OK;

    this->close();
}


void IlluminantPlaceChooser::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}
