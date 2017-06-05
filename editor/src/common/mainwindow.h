#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QCloseEvent;
class EditorManager;
class CommandManager;
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
    EditorManager *editor_manager;
    CommandManager *command_manager;
    Document *document;
};

#endif // MAINWINDOW_H
