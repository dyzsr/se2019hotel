#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  pipe(this)
{
  ui->setupUi(this);
  pipe.start();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_checkInButton_clicked()
{

}
