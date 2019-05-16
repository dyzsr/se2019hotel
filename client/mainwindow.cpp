#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::refresh(Room room)
{
  ui->DispTemp->setNum(room.temp);
  ui->DispSettemp->setNum(room.settemp);
  ui->DispWdspd->setNum(room.wdspd);
  ui->DispSetwdspd->setNum(room.setwdspd);
}

void MainWindow::showBilling(QVector<Billing> billings)
{

}

void MainWindow::showDetails(QVector<Billing> billings)
{

}
