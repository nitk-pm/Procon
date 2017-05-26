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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initSettings();

    auto scene = new Scene(101, 65);

    ui->view->setScene(scene);

    auto document = new Document(scene);

    auto vertex_plotter = new VertexPlotter();
    auto object_selector = new ObjectSelector();
    auto polygon_creator = new PolygonCreator();

    object_selector->setDeleteAction(ui->action_delete);

    EditorManager::instance()->registerEditor(ui->action_add_vertex, vertex_plotter);
    EditorManager::instance()->registerEditor(ui->action_select, object_selector);
    EditorManager::instance()->registerEditor(ui->action_create_polygon, polygon_creator);
    EditorManager::instance()->setDocument(document);
    EditorManager::instance()->connectScene(scene);

    CommandManager::instance()->setUndoAction(ui->action_undo);
    CommandManager::instance()->setRedoAction(ui->action_redo);
}

MainWindow::~MainWindow() {
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

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}
