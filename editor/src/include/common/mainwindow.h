#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QCloseEvent;
class QActionGroup;
class DataContainer;
class SelectMode;
class PlottingMode;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initData();
    void initUndoRedo();
    void initSettings();

protected:
    void closeEvent(QCloseEvent *event);

private:
    DataContainer  *container;
    Ui::MainWindow *ui;
    QActionGroup   *mode;
    SelectMode     *select_mode;
    PlottingMode   *plotting_mode;
};

#endif // MAINWINDOW_H
