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

private slots:
    void on_sXRot_sliderMoved(int position);
    void on_sYRot_sliderMoved(int position);
    void on_sZRot_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    Image img;
    QPixmap pixmap;
};

#endif // MAINWINDOW_H
