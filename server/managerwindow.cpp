#include "managerwindow.h"
#include "ui_managerwindow.h"

ManagerWindow::ManagerWindow(QWidget *parent) :
  QWidget(parent, Qt::Window),
  ui(new Ui::ManagerWindow)
{
  ui->setupUi(this);
  init();
}

ManagerWindow::~ManagerWindow()
{
  delete ui;
}

void ManagerWindow::init()
{
  ui->lb_verdict->hide();
}

void ManagerWindow::on_bt_signIn_clicked()
{
  QString name = ui->edit_adminname->text();
  QString pswd = ui->edit_adminpswd->text();
  bool success = emit sgn_signIn(name, pswd);
  if (success) {
    ui->stackedWidget->setCurrentIndex(1);
    ui->lb_verdict->hide();
  } else {
    ui->lb_verdict->show();
  }
}

void ManagerWindow::on_bt_back_clicked()
{
  bool success = emit sgn_signOut();
  if (success) {
    ui->stackedWidget->setCurrentIndex(0);
  }
}
