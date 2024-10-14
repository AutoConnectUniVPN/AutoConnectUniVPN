#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/UiManager.h"
#include "src/DataLoadingResult.h"
#include "libs/CiscoVPN/CiscoVPN.h"
#include "src/MainWindowExecutor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void _on_action_quit_triggered() const;
    void _on_action_reset_triggered() const;
    void _on_action_info_triggered() const;
    void _on_action_de_triggered() const;
    void _on_action_help_guide_triggered() const;
    void _on_action_help_get_seed_triggered() const;
    void _on_action_help_faq_triggered() const;
    void _on_action_help_bug_report_triggered() const;
    void _on_connect_button_clicked() const;
    void _on_disconnect_button_clicked() const;

private:
    Ui::MainWindow *ui;
    MainWindowExecutor executor;

	void connect_signals() const;
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
