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
  bool sgn_signOut();

  void sgn_getInfo(int &state, double &settemp, int &setwdspd);

  void sgn_sendRequest(int state, double settemp, int setwdspd);

public slots:
  void init(double minTemp, double maxTemp);
  void refresh(Room room);
  void disable();
  int slot_getRecoverSpeed();

private slots:
  void on_bt_signIn_clicked();
  void on_bt_signOut_clicked();
  void on_bt_getIn_clicked();
  void on_bt_tempUp_clicked();
  void on_bt_tempDown_clicked();
  void on_sb_settemp_valueChanged(int arg1);
  void on_bt_wdspdUp_clicked();
  void on_bt_wdspdDown_clicked();
  void on_sb_setwdspd_valueChanged(int arg1);
  void on_bt_state0_clicked(bool checked);
  void on_bt_state1_clicked(bool checked);

private:
  Ui::MainWindow *ui;
  bool gotIn;
  bool ac_on;
};

#endif // MAINWINDOW_H
