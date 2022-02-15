#include "sizechooser.hpp"
#include "ui_sizechooser.h"

#include <QErrorMessage>

SizeChooser::SizeChooser(QWidget *parent) : QDialog(parent), ui(new Ui::SizeChooser)
{
    ui->setupUi(this);
    this->setWindowTitle("Выбор размеров плоскости");
}

SizeChooser::~SizeChooser() { delete ui; }

void SizeChooser::on_buttonBox_accepted()
{
    QString gotText = this->ui->lineEdit->text();
    int heightValue = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    int widthValue = gotText.toInt();

    if (heightValue > 0 && widthValue > 0)
    {
        height = heightValue;
        width = widthValue;
    }
    else
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Были переданы некорректные значения.\n"
                         "При вводе убедитесь, что введённые зна"
                         "чения являются целочисленными положите"
                         "льными числами.");
    }
    this->close();
}

void SizeChooser::setWidth(int width_) { width = width_; }

int SizeChooser::getWidth() { return width; }

void SizeChooser::setheight(int height_) { height = height_; }

int SizeChooser::getheight() { return height; }
