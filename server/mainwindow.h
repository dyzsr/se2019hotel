#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../share/objects.h"

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  QStringList sgn_getUsrIds();
  QStringList sgn_getAvailUsrIds();
  QString sgn_getUsrId(int roomId);
  void sgn_openNewWindow(QWidget *parent);
  bool sgn_checkin(int roomd, QString usrId);
  bool sgn_checkout(int roomId);
  void sgn_askSimpleBill_clicked();
  void sgn_askDetailedBill_clicked();
  bool sgn_checkRoomIdValid(int roomId);

  bool sgn_signup(QString usrId, QString pswd);
  bool sgn_del(QString usrId, QString pswd);

  void sgn_setMode(int mode);

public slots:
  void init(int nr_room);

private slots:
  void on_bt_callManager_clicked();
  void on_bt_checkin_clicked();
  void on_bt_checkout_clicked();
  void on_bt_simpleBill_clicked();
  void on_bt_detailedBill_clicked();

  void on_bt_signup_clicked();

  void on_rb_0_clicked();
  void on_rb_1_clicked();
  void on_rb_2_clicked();
  void on_rb_3_clicked();
  void on_rb_4_clicked();
  void on_rb_5_clicked();
  void on_rb_6_clicked();
  void on_rb_7_clicked();
  void on_rb_8_clicked();
  void on_rb_9_clicked();

  void on_sb_roomId_valueChanged(int arg1);

  void refresh();

  void on_bt_del_clicked();

  void on_rb_down_clicked();

  void on_rb_up_clicked();

  void on_rb_auto_clicked();

private:
  Ui::MainWindow *ui;

  bool checkRoomIdValid();
};

#endif // MAINWINDOW_H
