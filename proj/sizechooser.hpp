#ifndef SIZECHOOSER_HPP
#define SIZECHOOSER_HPP

#include <QDialog>

namespace Ui
{
class SizeChooser;
}

class SizeChooser : public QDialog
{
    Q_OBJECT

public:
    explicit SizeChooser(QWidget *parent = nullptr);
    ~SizeChooser();

    void setWidth(int width_);
    int getWidth();

    int getheight();
    void setheight(int height_);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SizeChooser *ui;

    int width = -1;
    int height = -1;
};

#endif // SIZECHOOSER_HPP
