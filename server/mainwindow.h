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
  void sgn_openNewWindow(QWidget *parent);
  void sgn_checkout(int roomId);
  void sgn_getBillings(int roomId);
  void sgn_askSimpleBill_clicked();
  void sgn_askDetailedBill_clicked();
  bool sgn_checkRoomIdValid(int roomId);

private slots:
  void on_bt_callManager_clicked();
  void on_bt_checkout_clicked();
  void on_bt_simpleBill_clicked();
  void on_bt_detailedBill_clicked();

private:
  Ui::MainWindow *ui;

  bool checkRoomIdValid();
};

#endif // MAINWINDOW_H
