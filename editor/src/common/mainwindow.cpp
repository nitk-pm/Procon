#include "common/mainwindow.h"
#include "ui_window_layout.h"
#include "common/scene.h"
#include "command/command.h"
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>
#include <QtWidgets/QActionGroup>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    Scene *scene = new Scene();
    scene->createBackground(101, 65);
    ui->view->setScene(scene);

    connect(ui->action_undo, SIGNAL(triggered()), Command::stack, SLOT(undo()));
    connect(ui->action_redo, SIGNAL(triggered()), Command::stack, SLOT(redo()));
    connect(Command::stack, SIGNAL(canUndoChanged(bool)), ui->action_undo, SLOT(setEnabled(bool)));
    connect(Command::stack, SIGNAL(canRedoChanged(bool)), ui->action_redo, SLOT(setEnabled(bool)));

    operation = new QActionGroup(this);
    operation->setExclusive(true);
    operation->addAction(ui->action_select);
    operation->addAction(ui->action_add_vertex);
    operation->addAction(ui->action_create_polygon);


    QTimer::singleShot(0, [&]() {
        QSettings settings("setting.ini", QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        this->restoreGeometry(settings.value("main/geometry").toByteArray());
        this->restoreState(settings.value("main/windowState").toByteArray());
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}
