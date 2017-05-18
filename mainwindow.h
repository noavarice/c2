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

    void on_sQuality_sliderMoved(int position);

    void on_sbR_valueChanged(double arg1);

    void on_sbPhi_valueChanged(int arg1);

private:
    double r = 0.5;
    double phi = 20;
    int step = 1;

    Ui::MainWindow *ui;
    Image img;
    QPixmap pixmap;
};

#endif // MAINWINDOW_H
