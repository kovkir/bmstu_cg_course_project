#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QMouseEvent>

#include "usagefacade.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_createScene_clicked();
    void on_pushButton_sceneToInitianPosition_clicked();

    void on_pushButton_addModel_clicked();
    void on_pushButton_deleteModel_clicked();
    void on_pushButton_moveModel_clicked();

    void pictureDown();
    void pictureUp();
    void pictureLeft();
    void pictureRight();

    void pictureScaleUp();
    void pictureScaleDown();

    void pictureRotateXRight();
    void pictureRotateXLeft();
    void pictureRotateYRight();
    void pictureRotateYLeft();
    void pictureRotateZRight();
    void pictureRotateZLeft();

    void pictureToCenter();

    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_leftCircle_clicked();
    void on_pushButton_rightCircle_clicked();

    void on_pushButton_up_scene_clicked();
    void on_pushButton_down_scene_clicked();
    void on_pushButton_left_scene_clicked();
    void on_pushButton_right_scene_clicked();

    void on_pushButton_zoom_clicked();
    void on_pushButton_distance_clicked();

private:
    Ui::MainWindow *ui;
    UsageFacade *facade;
};

#endif // MAINWINDOW_H
