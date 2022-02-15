#ifndef MOVECHOOSER_H
#define MOVECHOOSER_H

#include <QDialog>

namespace Ui
{
class moveChooser;
}

class moveChooser : public QDialog
{
    Q_OBJECT

public:
    explicit moveChooser(QWidget *parent = nullptr);
    ~moveChooser();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    int getNewXCell();
    int getNewYCell();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::moveChooser *ui;

    int newXCell = -1;
    int newYCell = -1;
};

#endif // MOVECHOOSER_H
