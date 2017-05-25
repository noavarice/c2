#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void draw();
    void build();

private slots:
    void on_sXRot_sliderMoved(int position);
    void on_sYRot_sliderMoved(int position);
    void on_sZRot_sliderMoved(int position);

    void on_sbLightX_valueChanged(int val);
    void on_sbLightY_valueChanged(int val);
    void on_sbLightZ_valueChanged(int val);

    void on_doubleSpinBoxHeight_valueChanged(double newHeight);

    void on_doubleSpinBoxRadius_valueChanged(double newRadius);

    void on_spinBoxSidesCount_valueChanged(int newSidesCount);

private:
    Ui::MainWindow *ui;
    Image img;
    QPixmap pixmap;

    double height;
    double radius;
    int sidesCount;
};

#endif // MAINWINDOW_H
