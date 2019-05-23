#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentWidget(ui->page_frontdesk);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_bt_callManager_clicked()
{
  emit sgn_openNewWindow(this);
}

void MainWindow::on_bt_checkout_0_clicked()
{
  emit sgn_checkout(0);
  // TODO
}

void MainWindow::on_bt_billing_0_clicked()
{
  QVector<Billing> billings = emit sgn_getBillings(0);
  // TODO
}

void MainWindow::on_bt_details_0_clicked()
{
  QVector<Billing> billings = emit sgn_getBillings(0);
  // TODO
}
