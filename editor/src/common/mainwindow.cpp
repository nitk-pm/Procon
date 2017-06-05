#include "common/mainwindow.h"
#include "ui_window_layout.h"

#include "common/scene.h"
#include "commands/command_manager.h"
#include "models/document.h"
#include "editors/editor_manager.h"
#include "editors/vertex_plotter.h"
#include "editors/object_selector.h"
#include "editors/polygon_creator.h"

#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>
#include <QtCore/QFile>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initSettings();

    auto scene = new Scene(101, 65);
    ui->view->setScene(scene);

    document = new Document(scene);

    command_manager = new CommandManager(new QUndoStack());
    command_manager->setUndoAction(ui->action_undo);
    command_manager->setRedoAction(ui->action_redo);

    editor_manager = new EditorManager(document, command_manager, ui);
    editor_manager->registerEditor(ui->action_add_vertex, new VertexPlotter());
    editor_manager->registerEditor(ui->action_select, new ObjectSelector());
    editor_manager->registerEditor(ui->action_create_polygon, new PolygonCreator());
    editor_manager->selectEditorAt(0);

    connect(ui->action_new_file, SIGNAL(triggered()), this, SLOT(newFile()));
}

MainWindow::~MainWindow() {
    delete editor_manager;
    delete command_manager;
    delete document;
    delete ui;
}

void MainWindow::initSettings() {
    QTimer::singleShot(0, [&]() {
        QSettings settings("setting.ini", QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        this->restoreGeometry(settings.value("main/geometry").toByteArray());
        this->restoreState(settings.value("main/windowState").toByteArray());
    });
}

void MainWindow::saveFile() {

}

void MainWindow::openFile() {

}

void MainWindow::newFile() {
    command_manager->undoStack()->clear();
    document->clear();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}
