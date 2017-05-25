#include "common/mainwindow.h"
#include "ui_window_layout.h"

#include "common/scene.h"
#include "commands/command_manager.h"
#include "editors/editor_manager.h"
#include "editors/vertex_plotter.h"
#include "editors/object_selector.h"
#include "models/document.h"

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

    connect(object_selector, SIGNAL(setDeleteActionFlag(bool)), ui->action_delete, SLOT(setEnabled(bool)));
    connect(ui->action_delete, SIGNAL(triggered()), object_selector, SLOT(remove()));

    EditorManager::instance()->registerEditor(ui->action_add_vertex, vertex_plotter);
    EditorManager::instance()->registerEditor(ui->action_select, object_selector);
    EditorManager::instance()->setDocument(document);
    connect(EditorManager::instance(), SIGNAL(triggered(QAction*)), scene, SLOT(changeEditor(QAction*)));

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
