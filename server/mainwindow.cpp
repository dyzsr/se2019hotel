#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentWidget(ui->page_frontdesk);
  ui->lb_verdict->hide();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_bt_signIn_clicked()
{
  ui->stackedWidget->setCurrentWidget(ui->page_manager2);
}

void MainWindow::on_bt_back_clicked()
{
  ui->stackedWidget->setCurrentWidget(ui->page_manager1);
}

void MainWindow::on_bt_callManager_clicked()
{
}
