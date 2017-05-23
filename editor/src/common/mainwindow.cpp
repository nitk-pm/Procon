#include "common/mainwindow.h"
#include "ui_window_layout.h"

#include "common/scene.h"
#include "editors/editor_manager.h"
#include "editors/vertex_plotter.h"
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

    EditorManager::instance()->registerEditor(ui->action_add_vertex, new VertexPlotter(document));
    EditorManager::instance()->registerEditor(ui->action_select, 0);

    connect(EditorManager::instance(), SIGNAL(triggered(QAction*)), scene, SLOT(changeEditor(QAction*)));
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
