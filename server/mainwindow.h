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
  QString sgn_getUsrId(int roomId);
  void sgn_openNewWindow(QWidget *parent);
  void sgn_checkout(int roomId);
//  void sgn_getBillings(int roomId);
  void sgn_askSimpleBill_clicked();
  void sgn_askDetailedBill_clicked();
  bool sgn_checkRoomIdValid(int roomId);

public slots:
  void init(int nr_room);

private slots:
  void on_bt_callManager_clicked();
  void on_bt_checkout_clicked();
  void on_bt_simpleBill_clicked();
  void on_bt_detailedBill_clicked();

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

private:
  Ui::MainWindow *ui;

  bool checkRoomIdValid();
};

#endif // MAINWINDOW_H
