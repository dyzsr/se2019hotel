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
  QVector<Billing> sgn_getBillings(int roomId);

private slots:
  void on_bt_callManager_clicked();

  void on_bt_checkout_0_clicked();

  // 触发信号 显示账单
  void on_bt_billing_0_clicked();

  // 触发信号 显示详单
  void on_bt_details_0_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
