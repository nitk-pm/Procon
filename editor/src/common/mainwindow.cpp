#include "common/mainwindow.h"
#include "ui_window_layout.h"
#include "common/scene.h"
#include "common/data_container.h"
#include "mode/plotting_mode.h"
#include "mode/select_mode.h"
#include "command/command.h"

#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>
#include <QtWidgets/QActionGroup>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initData();
    initUndoRedo();
    initSettings();
}

MainWindow::~MainWindow() {
    delete container;
    delete mode;
    delete ui;
}

void MainWindow::initData() {
    Scene *scene = new Scene();
    ui->view->setScene(scene);

    mode = new QActionGroup(this);
    mode->setExclusive(true);
    mode->addAction(ui->action_select);
    mode->addAction(ui->action_add_vertex);
    mode->addAction(ui->action_create_polygon);
    connect(mode, SIGNAL(triggered(QAction*)), scene, SLOT(changeMode(QAction*)));

    container = new DataContainer(scene, 101, 65);

    QVariant select_mode_variant;
    auto select_mode = new SelectMode(container);
    select_mode_variant.setValue(select_mode);

    QVariant plotting_mode_variant;
    auto plotting_mode = new PlottingMode(container);
    plotting_mode_variant.setValue(plotting_mode);

    ui->action_select->setData(select_mode_variant);
    ui->action_add_vertex->setData(plotting_mode_variant);

    connect(select_mode, SIGNAL(setDeleteActionFlag(bool)), ui->action_delete, SLOT(setEnabled(bool)));
}

void MainWindow::initUndoRedo() {
    connect(ui->action_undo, SIGNAL(triggered()), Command::stack, SLOT(undo()));
    connect(ui->action_redo, SIGNAL(triggered()), Command::stack, SLOT(redo()));
    connect(Command::stack, SIGNAL(canUndoChanged(bool)), ui->action_undo, SLOT(setEnabled(bool)));
    connect(Command::stack, SIGNAL(canRedoChanged(bool)), ui->action_redo, SLOT(setEnabled(bool)));
}

void MainWindow::initSettings() {
    QTimer::singleShot(0, [&]() {
        QSettings settings("setting.ini", QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        this->restoreGeometry(settings.value("main/geometry").toByteArray());
        this->restoreState(settings.value("main/windowState").toByteArray());
    });
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}
