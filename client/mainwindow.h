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
  void on_bt_ok_clicked();

  void on_bt_checkIn_clicked();

  void on_bt_checkOut_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
