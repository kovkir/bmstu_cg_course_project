#include "placechooser.h"
#include "ui_placehousechooser.h"
#include "ui_placetreechooser.h"
#include "ui_placeroadсhooser.h"
#include "ui_placecarсhooser.h"

#include <QErrorMessage>
#include <QDebug>


PlaceHouseChooser::PlaceHouseChooser(QWidget *parent) : QDialog(parent), ui(new Ui::PlaceHouseChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор расположения дома");
}

PlaceHouseChooser::~PlaceHouseChooser() { delete ui; }


void PlaceHouseChooser::setXCell(int xCell_) { xCell = xCell_; }

int PlaceHouseChooser::getXCell() { return xCell; }

void PlaceHouseChooser::setYCell(int yCell_) { yCell = yCell_; }

int PlaceHouseChooser::getYCell() { return yCell; }

int PlaceHouseChooser:: getModelLength() { return modelLength; }

int PlaceHouseChooser::getModelHeight() { return modelHeight; }


void PlaceHouseChooser::setHouseHeight(int houseHeight_) { houseHeight = houseHeight_; }

int PlaceHouseChooser::getHouseHeight() { return houseHeight; }


void PlaceHouseChooser::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    gotText = ui->lineEdit_3->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelLength = gotText.toInt();

    gotText = ui->lineEdit_4->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelHeight = gotText.toInt();

    gotText = ui->lineEdit_5->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    houseHeight = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Ошибка. Неверно введены данные.\n"
                         "Номера ячеек - целые положительные числа.");
        return;
    }
    else if (modelLength <= 0 || modelHeight <= 0 || houseHeight <=0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Ошибка. Неверно введены данные.\n"
                         "Размеры дома - целые положительные числа.");
        return;
    }
    else
        status = OK;

    this->close();
}

void PlaceHouseChooser::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}





PlaceTreeChooser::PlaceTreeChooser(QWidget *parent) : QDialog(parent), ui(new Ui::PlaceTreeChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор расположения дерева");
}

PlaceTreeChooser::~PlaceTreeChooser() { delete ui; }


void PlaceTreeChooser::setXCell(int xCell_) { xCell = xCell_; }

int PlaceTreeChooser::getXCell() { return xCell; }

void PlaceTreeChooser::setYCell(int yCell_) { yCell = yCell_; }

int PlaceTreeChooser::getYCell() { return yCell; }

int PlaceTreeChooser:: getModelLength() { return modelLength; }

int PlaceTreeChooser::getModelHeight() { return modelHeight; }


void PlaceTreeChooser::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
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

void PlaceTreeChooser::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}





PlaceRoadChooser::PlaceRoadChooser(QWidget *parent) : QDialog(parent), ui(new Ui::PlaceRoadChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор расположения дороги");
}

PlaceRoadChooser::~PlaceRoadChooser() { delete ui; }


void PlaceRoadChooser::setXCell(int xCell_) { xCell = xCell_; }

int PlaceRoadChooser::getXCell() { return xCell; }

void PlaceRoadChooser::setYCell(int yCell_) { yCell = yCell_; }

int PlaceRoadChooser::getYCell() { return yCell; }

int PlaceRoadChooser:: getModelLength() { return modelLength; }

int PlaceRoadChooser::getModelHeight() { return modelHeight; }

PlaceRoadChooser::checkBox PlaceRoadChooser::getDirection() { return direction; }


void PlaceRoadChooser::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    if (ui->radioButton->isChecked())
        direction = checkBox::XAXIS;
    else
        direction = checkBox::YAXIS;

    if (xCell <= 0 || yCell <= 0)
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

void PlaceRoadChooser::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}




PlaceCarChooser::PlaceCarChooser(QWidget *parent) : QDialog(parent), ui(new Ui::PlaceCarChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор расположения машины");
}

PlaceCarChooser::~PlaceCarChooser() { delete ui; }


void PlaceCarChooser::setXCell(int xCell_) { xCell = xCell_; }

int PlaceCarChooser::getXCell() { return xCell; }

void PlaceCarChooser::setYCell(int yCell_) { yCell = yCell_; }

int PlaceCarChooser::getYCell() { return yCell; }

int PlaceCarChooser:: getModelLength() { return modelLength; }

int PlaceCarChooser::getModelHeight() { return modelHeight; }

PlaceCarChooser::checkBox PlaceCarChooser::getDirection() { return direction; }


void PlaceCarChooser::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    if (ui->radioButton->isChecked())
        direction = checkBox::XAXIS;
    else
        direction = checkBox::YAXIS;

    if (xCell <= 0 || yCell <= 0)
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

void PlaceCarChooser::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}
