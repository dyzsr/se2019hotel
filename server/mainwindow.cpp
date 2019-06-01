#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#pragma execution_character_set("utf-8")

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

void MainWindow::init(int nr_room)
{
  int roomId = ui->sb_roomId->value();
  QString text = emit sgn_getUsrId(roomId);

  if (text.isEmpty()) {
    ui->bt_checkout->setEnabled(false);
    ui->bt_checkin->setEnabled(true);
    ui->lb_usrId->setText("无人");
  }
  else {
    ui->bt_checkout->setEnabled(true);
    ui->bt_checkin->setEnabled(false);
    ui->lb_usrId->setText(text);
  }

  ui->lb_err->hide();
  ui->lb_dup->hide();
  QStringList availUsrIds = emit sgn_getAvailUsrIds();
  ui->cb_users->addItems(availUsrIds);

  QStringList usrIds = emit sgn_getUsrIds();
  ui->lw_usrIds->addItems(usrIds);

  if (nr_room > 0) ui->sb_roomId->setRange(0, nr_room - 1);
  else ui->sb_roomId->setEnabled(false);

  if (nr_room <= 0) ui->rb_0->setEnabled(false);
  if (nr_room <= 1) ui->rb_1->setEnabled(false);
  if (nr_room <= 2) ui->rb_2->setEnabled(false);
  if (nr_room <= 3) ui->rb_3->setEnabled(false);
  if (nr_room <= 4) ui->rb_4->setEnabled(false);
  if (nr_room <= 5) ui->rb_5->setEnabled(false);
  if (nr_room <= 6) ui->rb_6->setEnabled(false);
  if (nr_room <= 7) ui->rb_7->setEnabled(false);
  if (nr_room <= 8) ui->rb_8->setEnabled(false);
  if (nr_room <= 9) ui->rb_9->setEnabled(false);
}

void MainWindow::refresh()
{
  int roomId = ui->sb_roomId->value();
  QString text = emit sgn_getUsrId(roomId);

  if (text.isEmpty()) {
    ui->bt_checkout->setEnabled(false);
    ui->bt_checkin->setEnabled(true);
    ui->lb_usrId->setText("无人");
  }
  else {
    ui->bt_checkout->setEnabled(true);
    ui->bt_checkin->setEnabled(false);
    ui->lb_usrId->setText(text);
  }

  QStringList availUsrIds = emit sgn_getAvailUsrIds();
  ui->cb_users->clear();
  ui->cb_users->addItems(availUsrIds);

  QStringList usrIds = emit sgn_getUsrIds();
  ui->lw_usrIds->clear();
  ui->lw_usrIds->addItems(usrIds);
}

void MainWindow::on_bt_callManager_clicked()
{
  emit sgn_openNewWindow(this);
}

bool MainWindow::checkRoomIdValid()
{
  bool isValid = true;
  int roomId = ui->sb_roomId->value();
  if (isValid)
  {
    isValid = emit sgn_checkRoomIdValid(roomId);
  }
  return isValid;
}

void MainWindow::on_bt_checkout_clicked()
{
  if (!checkRoomIdValid())
    return;
  bool success = emit sgn_checkout(ui->sb_roomId->value());
  if (success) {
//    int roomId = ui->sb_roomId->value();
    refresh();
  }
}

void MainWindow::on_bt_simpleBill_clicked()
{
  if (!checkRoomIdValid())
    return;
  emit sgn_askSimpleBill_clicked();
}

void MainWindow::on_bt_detailedBill_clicked()
{
  if (!checkRoomIdValid())
    return;
  emit sgn_askDetailedBill_clicked();
}

void MainWindow::on_bt_checkin_clicked()
{
  int roomId = ui->sb_roomId->value();
  QString usrId = ui->cb_users->currentText();
  qDebug() << usrId << roomId;
  bool success = emit sgn_checkin(roomId, usrId);
  if (!success) {
    ui->lb_err->show();
  } else {
    ui->lb_err->hide();
    refresh();
  }
}

void MainWindow::on_bt_signup_clicked()
{
  bool success = emit sgn_signup(ui->le_usrid->text(), ui->le_usrpw->text());
  if (!success) {
    ui->lb_dup->setText("用户名重复");
    ui->lb_dup->show();
  }
  else {
    ui->lb_dup->hide();
    refresh();
  }
}

void MainWindow::on_bt_del_clicked()
{
  bool success = emit sgn_del(ui->le_usrid->text(), ui->le_usrpw->text());
  if (!success) {
    ui->lb_dup->setText("用户名不存在");
    ui->lb_dup->show();
  }
  else {
    ui->lb_dup->hide();
    refresh();
  }
}

void MainWindow::on_rb_0_clicked()
{
   ui->sb_roomId->setValue(0);
}

void MainWindow::on_rb_1_clicked()
{
   ui->sb_roomId->setValue(1);
}

void MainWindow::on_rb_2_clicked()
{
   ui->sb_roomId->setValue(2);
}

void MainWindow::on_rb_3_clicked()
{
   ui->sb_roomId->setValue(3);
}

void MainWindow::on_rb_4_clicked()
{
   ui->sb_roomId->setValue(4);
}

void MainWindow::on_rb_5_clicked()
{
   ui->sb_roomId->setValue(5);
}

void MainWindow::on_rb_6_clicked()
{
   ui->sb_roomId->setValue(6);
}

void MainWindow::on_rb_7_clicked()
{
   ui->sb_roomId->setValue(7);
}

void MainWindow::on_rb_8_clicked()
{
   ui->sb_roomId->setValue(8);
}

void MainWindow::on_rb_9_clicked()
{
   ui->sb_roomId->setValue(9);
}

void MainWindow::on_sb_roomId_valueChanged(int roomId)
{
  QString text = sgn_getUsrId(roomId);
  if (text.isEmpty()) {
    ui->bt_checkout->setEnabled(false);
    ui->bt_checkin->setEnabled(true);
    ui->lb_usrId->setText("无人");
  }
  else {
    ui->bt_checkout->setEnabled(true);
    ui->bt_checkin->setEnabled(false);
    ui->lb_usrId->setText(text);
  }

  switch (roomId) {
  case 0: ui->rb_0->setChecked(true); break;
  case 1: ui->rb_1->setChecked(true); break;
  case 2: ui->rb_2->setChecked(true); break;
  case 3: ui->rb_3->setChecked(true); break;
  case 4: ui->rb_4->setChecked(true); break;
  case 5: ui->rb_5->setChecked(true); break;
  case 6: ui->rb_6->setChecked(true); break;
  case 7: ui->rb_7->setChecked(true); break;
  case 8: ui->rb_8->setChecked(true); break;
  case 9: ui->rb_9->setChecked(true); break;
  }
}
