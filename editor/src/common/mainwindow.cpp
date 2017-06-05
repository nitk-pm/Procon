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
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initSettings();

    auto scene = new Scene(101, 65);
    ui->view->setScene(scene);

    document = new Document(scene);
    document->setFilename(tr("無題のドキュメント"));

    command_manager = new CommandManager(new QUndoStack());
    command_manager->setUndoAction(ui->action_undo);
    command_manager->setRedoAction(ui->action_redo);

    editor_manager = new EditorManager(document, command_manager, ui);
    editor_manager->registerEditor(ui->action_add_vertex, new VertexPlotter());
    editor_manager->registerEditor(ui->action_select, new ObjectSelector());
    editor_manager->registerEditor(ui->action_create_polygon, new PolygonCreator());
    editor_manager->selectEditorAt(0);

    connect(ui->action_new_file, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->action_save_file, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_save_as_file, SIGNAL(triggered()), this, SLOT(saveAs()));
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

int MainWindow::checkUpdatedDocument() {
    if (document->isUpdated()) {
        QMessageBox box(this);
        box.setWindowTitle(tr("確認ダイアログ"));
        box.setText(document->filename() + tr("は変更されています。\n保存しますか?"));
        box.setStandardButtons(QMessageBox::Save | QMessageBox::SaveAll | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Save);
        return box.exec();
    }

    return QMessageBox::NoButton;
}

void MainWindow::save() {
    QFile file(document->filename() + ".txt");
    if (file.exists()) write(file);
    else saveAs();
}

void MainWindow::saveAs() {
    QString filename = QFileDialog::getSaveFileName(this,tr("名前を付けて保存"),document->filename(),tr("テキスト (*.txt);;すべてのファイル (*)"));
    if (filename.isEmpty()) return;
    else {
        document->setFilename(filename.split('.').front());
        QFile file(document->filename() + ".txt");
        write(file);
    }
}

void MainWindow::newFile() {
    int message = checkUpdatedDocument();
    switch(message) {
        case QMessageBox::Save:
            save();
            reset();
            break;

        case QMessageBox::SaveAll:
            saveAs();
            reset();
            break;
    };
}

void MainWindow::write(QFile &file) {
    QString data = document->serialize();
    if (!data.isEmpty()) {
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << data;
    }
    else {
        QMessageBox box(this);
        box.setIcon(QMessageBox::Warning);
        box.setWindowTitle(tr("warning"));
        box.setText(tr("保存出来るデータがありません。"));
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
    }
}

void MainWindow::reset() {
    command_manager->undoStack()->clear();
    document->setFilename(tr("無題のドキュメント"));
    document->clear();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    int message = checkUpdatedDocument();
    switch(message) {
        case QMessageBox::Save:
            save();
            break;

        case QMessageBox::SaveAll:
            saveAs();
            break;
    }
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}
