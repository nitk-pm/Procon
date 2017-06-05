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

    document = new Document(scene);
    vertex_plotter = new VertexPlotter();
    object_selector = new ObjectSelector();
    polygon_creator = new PolygonCreator();
    stack = new QUndoStack();

    object_selector->setDeleteAction(ui->action_delete);

    EditorManager::instance()->registerEditor(ui->action_add_vertex, vertex_plotter);
    EditorManager::instance()->registerEditor(ui->action_select, object_selector);
    EditorManager::instance()->registerEditor(ui->action_create_polygon, polygon_creator);
    EditorManager::instance()->setScene(scene);
    EditorManager::instance()->setDocument(document);

    EditorManager::instance()->selectEditorAt(0);

    CommandManager::instance()->setUndoStack(stack);
    CommandManager::instance()->setUndoAction(ui->action_undo);
    CommandManager::instance()->setRedoAction(ui->action_redo);
}

MainWindow::~MainWindow() {
    delete stack;
    delete vertex_plotter;
    delete object_selector;
    delete polygon_creator;
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

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}
