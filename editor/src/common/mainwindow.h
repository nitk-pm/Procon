#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QFile;
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
    int checkUpdatedDocument();
    void open();
    void save();
    void saveAs();
    void newFile();
    void write(QFile &file);
    void reset();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    EditorManager *editor_manager;
    CommandManager *command_manager;
    Document *document;
};

#endif // MAINWINDOW_H
