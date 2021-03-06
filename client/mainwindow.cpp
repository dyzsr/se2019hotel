﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  ui->lb_verdict->hide();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::init(double minTemp, double maxTemp)
{
  ui->sb_settemp->setMinimum(static_cast<int>(minTemp));
  ui->sb_settemp->setMaximum(static_cast<int>(maxTemp));

  ui->bt_state1->setEnabled(false);
  ui->bt_state0->setEnabled(true);
  gotIn = false;
}

void MainWindow::refresh(Room room)
{
  if (!gotIn) {
    ui->bt_tempUp->setEnabled(ui->sb_settemp->value() < ui->sb_settemp->maximum());
    ui->bt_tempDown->setEnabled(ui->sb_settemp->value() > ui->sb_settemp->minimum());
    ui->bt_wdspdUp->setEnabled(ui->sb_setwdspd->value() < ui->sb_setwdspd->maximum());
    ui->bt_wdspdDown->setEnabled(ui->sb_setwdspd->value() > ui->sb_setwdspd->minimum());
    ui->sb_settemp->setEnabled(true);
    ui->sb_setwdspd->setEnabled(true);

    ui->sb_settemp->setValue(static_cast<int>(slot_getRecoverSpeed()));
    ui->sb_setwdspd->setValue(room.wdspd);
    gotIn = true;
  }

  ui->lcd_temp->display(room.temp);
  ui->lcd_settemp->display(room.settemp);
  ui->lcd_wdspd->display(room.wdspd);
  ui->lcd_setwdspd->display("-");
  ui->lb_fee->setText(QString::number(room.cost, 'f', 4));
  ui->lb_feeRate->setText(QString::number(room.pwr, 'f', 4));

  if (room.mode == false)
    ui->lb_mode->setText("制冷");
  else
    ui->lb_mode->setText("制热");

  if (room.state == 0) {
    ui->lb_state->setText("关机");
  }
  else if (room.state == 1) {
    ui->lb_state->setText("运行");
  }
  else if (room.state == 2) {
    ui->lb_state->setText("待机");
  }
  else if (room.state == 3) {
    ui->lb_state->setText("调度");
  }

  if (room.state == 0)
  {
      ui->bt_state0->setEnabled(false);
      ui->bt_state1->setEnabled(true);
  }
  else
  {
      ui->bt_state0->setEnabled(true);
      ui->bt_state1->setEnabled(false);
  }
  ui->lb_roomId->setNum(room.roomId);
}

void MainWindow::disable()
{
  gotIn = false;
  ui->bt_state0->setEnabled(false);
  ui->bt_state1->setEnabled(false);
  ui->bt_tempUp->setEnabled(false);
  ui->bt_tempDown->setEnabled(false);
  ui->bt_wdspdUp->setEnabled(false);
  ui->bt_wdspdDown->setEnabled(false);
  ui->sb_settemp->setEnabled(false);
  ui->sb_setwdspd->setEnabled(false);

  ui->lb_roomId->setText("-");
  ui->lb_state->setText("-");
  ui->lb_mode->setText("-");
  ui->lb_fee->setText("-");
  ui->lb_feeRate->setText("-");
  ui->lcd_temp->display("-");
  ui->lcd_settemp->display("-");
  ui->lcd_wdspd->display("-");
//  ui->lcd_setwdspd->display("-");
}

void MainWindow::on_bt_signIn_clicked()
{
  bool success = emit sgn_signIn(ui->edit_username->text(),
                                 ui->edit_password->text());
  if (success) {
    ui->sb_settemp->setValue(slot_getRecoverSpeed());
    ui->lb_verdict->hide();
    ui->stackedWidget->setCurrentIndex(1);
  } else {
    ui->lb_verdict->show();
  }
}

void MainWindow::on_bt_signOut_clicked()
{
  bool success = emit sgn_signOut();
  if (success) {
    ui->stackedWidget->setCurrentIndex(0);
    ui->edit_password->setText("");
  }
}

void MainWindow::on_bt_tempUp_clicked()
{
  int temp = ui->sb_settemp->value();
  ui->sb_settemp->setValue(temp + 1);
}

void MainWindow::on_bt_tempDown_clicked()
{
  int temp = ui->sb_settemp->value();
  ui->sb_settemp->setValue(temp - 1);
}

void MainWindow::on_sb_settemp_valueChanged(int temp)
{
  ui->bt_tempUp->setEnabled(temp < ui->sb_settemp->maximum());
  ui->bt_tempDown->setEnabled(temp > ui->sb_settemp->minimum());
  if (gotIn) {
    emit sgn_sendRequest(ui->bt_state0->isEnabled(),
                         ui->sb_settemp->value(),
                         ui->sb_setwdspd->value());
  }
}

void MainWindow::on_bt_wdspdUp_clicked()
{
  int wdspd = ui->sb_setwdspd->value();
  ui->sb_setwdspd->setValue(wdspd + 1);
}

void MainWindow::on_bt_wdspdDown_clicked()
{
  int wdspd = ui->sb_setwdspd->value();
  ui->sb_setwdspd->setValue(wdspd - 1);
}

void MainWindow::on_sb_setwdspd_valueChanged(int wdspd)
{
  ui->bt_wdspdUp->setEnabled(wdspd < ui->sb_setwdspd->maximum());
  ui->bt_wdspdDown->setEnabled(wdspd > ui->sb_setwdspd->minimum());
  if (gotIn) {
    emit sgn_sendRequest(ui->bt_state0->isEnabled(),
                         ui->sb_settemp->value(),
                         ui->sb_setwdspd->value());
  }
}

void MainWindow::on_bt_state0_clicked(bool checked)
{
  if (!checked) {
    ui->bt_state0->setEnabled(false);
    ui->bt_state1->setEnabled(true);
    ac_on = false;
    // 关机
    if (gotIn) {
      emit sgn_sendRequest(ui->bt_state0->isEnabled(),
                           ui->sb_settemp->value(),
                           ui->sb_setwdspd->value());
    }
  }
}

void MainWindow::on_bt_state1_clicked(bool checked)
{
  if (!checked) {
    ui->bt_state0->setEnabled(true);
    ui->bt_state1->setEnabled(false);
    ac_on = true;
    // 开机
    if (gotIn) {
      emit sgn_sendRequest(ui->bt_state0->isEnabled(),
                           ui->sb_settemp->value(),
                           ui->sb_setwdspd->value());
    }
  }
}

int MainWindow::slot_getRecoverSpeed()
{
  return ui->sb_recoverSpeed->value();
}
