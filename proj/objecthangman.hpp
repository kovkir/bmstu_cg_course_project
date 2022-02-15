#ifndef OBJECTHANGMAN_HPP
#define OBJECTHANGMAN_HPP

#include <QDialog>
#include "objects.hpp"

namespace Ui
{
class ObjectHangman;
}

class ObjectHangman : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectHangman(CellScene *scene_, QWidget *parent = nullptr);
    ~ObjectHangman();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ObjectHangman *ui;
    CellScene *scene;

    void recalculationModelsNum();
};

#endif // OBJECTHANGMAN_HPP
