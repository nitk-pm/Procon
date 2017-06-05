#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QCloseEvent;
class QUndoStack;
class VertexPlotter;
class ObjectSelector;
class PolygonCreator;
class Document;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initSettings();

public slots:
    void saveFile();
    void openFile();
    void newFile();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QUndoStack *stack;
    VertexPlotter *vertex_plotter;
    ObjectSelector *object_selector;
    PolygonCreator *polygon_creator;
    Document *document;
};

#endif // MAINWINDOW_H
