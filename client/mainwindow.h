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
  bool sgn_signIn(QString usrId, QString passwd);

  void sgn_incTemp();
  void sgn_decTemp();

public slots:
  void refresh(Room room);
  void showBilling(QVector<Billing> billings);
  void showDetails(QVector<Billing> billings);

private slots:
  void on_bt_signIn_clicked();

  void on_bt_checkIn_clicked();

  void on_bt_checkOut_clicked();

  void on_bt_tempUp_clicked();

  void on_bt_tempDown_clicked();

  void on_sb_settemp_valueChanged(int arg1);

  void on_bt_wdspdUp_clicked();

  void on_bt_wdspdDown_clicked();

  void on_sb_setwdspd_valueChanged(int arg1);

  void on_bt_state0_clicked(bool checked);

  void on_bt_state1_clicked(bool checked);

  void on_bt_state2_clicked(bool checked);

  void on_bt_signOut_clicked();


private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
