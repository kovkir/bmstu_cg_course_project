#ifndef PLACECHOOSER_H
#define PLACECHOOSER_H

#include <QDialog>

namespace Ui
{
class PlaceHouseChooser;
class PlaceTreeChooser;
class PlaceRoadChooser;
class PlaceCarChooser;
}

class PlaceHouseChooser : public QDialog
{
    Q_OBJECT

public:
    explicit PlaceHouseChooser(QWidget *parent = nullptr);
    ~PlaceHouseChooser();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelHeight();

    void setHouseHeight(int houseHeight_);
    int  getHouseHeight();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PlaceHouseChooser *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelHeight = 0;

    int houseHeight = 0;
};


class PlaceTreeChooser : public QDialog
{
    Q_OBJECT

public:
    explicit PlaceTreeChooser(QWidget *parent = nullptr);
    ~PlaceTreeChooser();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelHeight();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PlaceTreeChooser *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 1;
    int modelHeight = 1;
};


class PlaceRoadChooser : public QDialog
{
    Q_OBJECT

public:
    explicit PlaceRoadChooser(QWidget *parent = nullptr);
    ~PlaceRoadChooser();

    enum checkBox
    {
        XAXIS,
        YAXIS
    };

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelHeight();

    PlaceRoadChooser::checkBox getDirection();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PlaceRoadChooser *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 1;
    int modelHeight = 1;

    checkBox direction;
};

class PlaceCarChooser : public QDialog
{
    Q_OBJECT

public:
    explicit PlaceCarChooser(QWidget *parent = nullptr);
    ~PlaceCarChooser();

    enum checkBox
    {
        XAXIS,
        YAXIS
    };

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelHeight();

    PlaceCarChooser::checkBox getDirection();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PlaceCarChooser *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelHeight = 0;

    checkBox direction;
};

#endif // PLACECHOOSER_H
