#include "managerwindow.h"
#include "ui_managerwindow.h"

ManagerWindow::ManagerWindow(QWidget *parent) :
  QWidget(parent, Qt::Window),
  ui(new Ui::ManagerWindow)
{
  ui->setupUi(this);
}

ManagerWindow::~ManagerWindow()
{
  delete ui;
}
