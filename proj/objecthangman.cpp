#include "objecthangman.hpp"
#include "ui_objecthangman.h"

#include <QErrorMessage>
#include <QDebug>

ObjectHangman::ObjectHangman(CellScene *scene_, QWidget *parent)
    : QDialog(parent), ui(new Ui::ObjectHangman)
{
    ui->setupUi(this);
    this->setWindowTitle("Удаление объектов сцены");

    scene = scene_;

    PolModel model;
    PolModel::model_t modelType_;
    int count = 0;

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        modelType_ = model.getModelType();

        if (modelType_ == PolModel::model_t::House || \
            modelType_ == PolModel::model_t::treeFoliage || \
            modelType_ == PolModel::model_t::roadAsphalt || \
            modelType_ == PolModel::model_t::Car)
        {
            count++;
            ui->listWidget->addItem(
                    QString::number(count) + ". " +
                                    scene->getModel(i).getName()       + " - (" +
                    QString::number(scene->getModel(i).getUsedXCell() + 1) + "; " +
                    QString::number(scene->getModel(i).getUsedYCell() + 1) + ")");
        }
    }

    for (size_t i = 0; i < scene->getIllumNum(); i++)
        ui->listWidget->addItem(
                    QString::number(count + i + 1)                       + ". Источ. света (" +
                    QString::number(scene->getIlluminant(i).getXAngle()) + "°; " +
                    QString::number(scene->getIlluminant(i).getYAngle()) + "°)");
}

ObjectHangman::~ObjectHangman() { delete ui; }

void ObjectHangman::on_pushButton_clicked()
{
    size_t curRow = size_t(this->ui->listWidget->currentRow());
    if (curRow < 0)
        return;

    size_t modelsNum     = scene->getModelsNum();
    size_t realModelsNum = scene->getRealModelsNum();
    bool flag = false;
    PolModel model;
    PolModel::model_t modelType_;

    if (curRow < realModelsNum)
    {
        for (size_t i = 0; i < modelsNum; i++)
        {
             model = scene->getModel(i);

             if (model.getModelNum() == curRow)
             {
                 modelType_ = model.getModelType();

                 if ((modelType_ == PolModel::model_t::roadAsphalt || \
                      modelType_ == PolModel::model_t::roadStripe) && \
                      scene->getUsedCells()[model.getUsedYCell()][model.getUsedXCell()] == 4)
                 {
                     close();
                     QErrorMessage *err = new QErrorMessage();
                     err->showMessage("Нельзя удалить дорогу, так как на ней расположена машина");
                     return;
                 }
                 else
                 {
                     scene->deleteModel(i);

                     flag = true;
                     modelsNum --;
                     i--;
                 }
             }
             else if (flag)
             {
                 break;
             }
        }

        recalculationModelsNum();
    }
    else
    {
        scene->deleteIlluminant(curRow - realModelsNum);
    }

    close();
}


void ObjectHangman::recalculationModelsNum()
{
    size_t realModelsNum = scene->getRealModelsNum();
    PolModel model;
    PolModel::model_t modelType_;
    int cur = 0, border = 0;

    for (size_t i = 0; i < realModelsNum; i++)
    {
        model = scene->getModel(cur);
        modelType_ = model.getModelType();

        if (modelType_ == PolModel::model_t::House || \
            modelType_ == PolModel::model_t::Car)
        {
            border = 3;
        }
        else
        {
            border = 2;
        }

        for (int j = 0; j < border; j++)
        {
            scene->getModel(cur).setModelNum(i);
            cur++;
        }
    }
}
