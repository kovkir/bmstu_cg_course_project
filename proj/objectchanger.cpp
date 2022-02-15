#include "objectchanger.hpp"
#include "ui_objectchanger.h"

#include "movechooser.hpp"

#include <QErrorMessage>
#include <QDebug>

ObjectChanger::ObjectChanger(UsageFacade *facade_, QWidget *parent)
    : QDialog(parent), ui(new Ui::ObjectChanger)
{
    ui->setupUi(this);
    this->setWindowTitle("Перемещение объектов сцены");

    facade = facade_;
    CellScene *scene = facade->getScene();

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
                                    scene->getModel(i).getName()      + " - (" +
                    QString::number(scene->getModel(i).getUsedXCell() + 1) + "; " +
                    QString::number(scene->getModel(i).getUsedYCell() + 1) + ")");
        }
    }
}

ObjectChanger::~ObjectChanger() { delete ui; }

void ObjectChanger::on_pushButton_clicked()
{
    size_t curRow = size_t(this->ui->listWidget->currentRow());
    if (curRow < 0)
        return;

    CellScene *scene     = facade->getScene();
    size_t modelsNum     = scene->getModelsNum();
    PolModel model;
    PolModel::model_t modelType_;
    bool flag = false;

    moveChooser moveChooserWindow(nullptr);
    moveChooserWindow.setModal(true);
    moveChooserWindow.exec();

    if (moveChooserWindow.status == moveChooser::OK)
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
                     err->showMessage("Нельзя переместить дорогу, так как на ней расположена машина");
                     return;
                 }
                 else
                 {
                     scene->clearUsedCells(i);
                     int retCode = changeModel(scene->getModel(i),
                                 moveChooserWindow.getNewXCell(),
                                 moveChooserWindow.getNewYCell());

                     if (retCode == 1)
                     {
                         close();
                         scene->markUsedCells(i);
                         QErrorMessage *err = new QErrorMessage();
                         err->showMessage("Некоторые из выбранных ячеек заняты");
                         return;
                     }
                     else if(retCode == 2)
                     {
                         close();
                         scene->markUsedCells(i);
                         QErrorMessage *err = new QErrorMessage();
                         err->showMessage("Объект не влезает в сцену");
                         return;
                     }
                     else if(retCode == 3)
                     {
                         close();
                         scene->markUsedCells(i);
                         QErrorMessage *err = new QErrorMessage();
                         err->showMessage("Машины разрешено ставить только на доргах");
                         return;
                     }
                     else if(retCode == 4)
                     {
                         close();
                         scene->markUsedCells(i);
                         QErrorMessage *err = new QErrorMessage();
                         err->showMessage("Дороги не должны прилегать к дому");
                         return;
                     }
                     else
                     {
                         scene->deleteModel(i);
                         scene->markUsedCells(scene->getModelsNum() - 1);
                         scene->printUsedCells();
                     }

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
        return;

    close();
}

int ObjectChanger::changeModel(PolModel &model, int newXCell, int newYCell)
{
    int retCode = 0;

    PolModel::model_t modelType_ = model.getModelType();

    if (modelType_ == PolModel::model_t::House)
    {
        retCode = facade->addHouse(newXCell, newYCell,
                                   model.getWidthModel(),
                                   model.getHeightModel(),
                                   model.getHouseHeight());
    }
    else if (modelType_ == PolModel::model_t::treeFoliage)
    {
        retCode = facade->addTree(newXCell, newYCell);
    }
    else if (modelType_ == PolModel::model_t::roadAsphalt)
    {
        retCode = facade->addRoad(newXCell, newYCell,
                                  model.getDirectionRoad());
    }
    else if (modelType_ == PolModel::model_t::Car)
    {
        retCode = facade->addCar(newXCell, newYCell,
                                  model.getDirectionCar());
    }

    return retCode;
}

void ObjectChanger::recalculationModelsNum()
{
    CellScene *scene     = facade->getScene();
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
