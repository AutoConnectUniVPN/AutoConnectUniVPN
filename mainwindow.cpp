#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialogs/InfoDialog/infodialog.h"
#include "dialogs/BugReportDialog/bugreportdialog.h"
#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), executor(ui) {
    ui->setupUi(this);
    this->resize(370, 550);
    executor.init_ui();
	connect_signals();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::connect_signals() const{
	// make buttons clickable
	auto action_quit = connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(_on_action_quit_triggered()));
	auto action_reset = connect(ui->actionReset, SIGNAL(triggered(bool)), this, SLOT(_on_action_reset_triggered()));
	auto action_info = connect(ui->actionInfo, SIGNAL(triggered(bool)), this, SLOT(_on_action_info_triggered()));
	auto action_de = connect(ui->actionDeutsch_DE, SIGNAL(triggered(bool)), this, SLOT(_on_action_de_triggered()));
	auto action_help_guide = connect(ui->actionHelpGuide, SIGNAL(triggered(bool)), this, SLOT(_on_action_help_guide_triggered()));
	auto action_help_get_seed = connect(ui->actionHelpGet2FASeed, SIGNAL(triggered(bool)), this, SLOT(_on_action_help_get_seed_triggered()));
	auto action_help_faq = connect(ui->actionHelpFAQ, SIGNAL(triggered(bool)), this, SLOT(_on_action_help_faq_triggered()));
	auto action_help_bug_report = connect(ui->actionReportBug, SIGNAL(triggered(bool)), this, SLOT(_on_action_help_bug_report_triggered()));
	auto connect_button = connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(_on_connect_button_clicked()));
	auto disconnect_button = connect(ui->disconnectButton, SIGNAL(clicked(bool)), this, SLOT(_on_disconnect_button_clicked()));
	auto to_log_button = connect(ui->toLogButton, &QPushButton::clicked, this, [this](){ this->executor.ui_manager.on_to_log_button_clicked();});
	auto to_data_input_button = connect(ui->toDataInputButton, &QPushButton::clicked, this, [this](){ this->executor.ui_manager.on_to_data_input_button_clicked();});
	// create timer to load cisco status not before main window is shown
	QTimer::singleShot(500, this, [this](){executor.load_cisco_vpn_status();});
}


void MainWindow::_on_action_quit_triggered() const {
    QApplication::quit();
}

void MainWindow::_on_action_reset_triggered() const {
    executor.on_action_reset_triggered();
}

void MainWindow::_on_connect_button_clicked() const {
    executor.on_connect_button_clicked();
}

void MainWindow::_on_disconnect_button_clicked() const {
    executor.on_disconnect_button_clicked();
}

void MainWindow::_on_action_info_triggered() const {
    // open info dialog
    InfoDialog info_dialog(ui->rootWidget);
    info_dialog.show();
    info_dialog.exec();  // wait until closed
}

void MainWindow::_on_action_de_triggered() const {
    QMessageBox::information(ui->rootWidget, " ", "Not available yet");
}

void MainWindow::_on_action_help_guide_triggered() const {
    QDesktopServices::openUrl(QUrl("https://github.com/AutoConnectUniVPN/AutoConnectUniVPN#autoconnectunivpn"));
}

void MainWindow::_on_action_help_get_seed_triggered() const {
    QDesktopServices::openUrl(QUrl("https://github.com/AutoConnectUniVPN/AutoConnectUniVPN#faq"));
}

void MainWindow::_on_action_help_faq_triggered() const {
    QDesktopServices::openUrl(QUrl("https://github.com/AutoConnectUniVPN/AutoConnectUniVPN#faq"));
}

void MainWindow::_on_action_help_bug_report_triggered() const {
    // open info dialog
    BugReportDialog bug_report_dialog(ui->rootWidget);
    bug_report_dialog.show();
    bug_report_dialog.exec();  // wait until closed
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
    // connect, if enter pressed and connect button is enabled
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        ui->connectButton->animateClick();
    }
}



