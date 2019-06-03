#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
  class ManagerWindow;
}

class ManagerWindow : public QWidget
{
  Q_OBJECT

public:
  explicit ManagerWindow(QWidget *parent = nullptr);
  ~ManagerWindow();

  void init();
  void showManagerFunc(QVector<QString> data);

signals:
  bool sgn_signIn(QString name, QString pswd);
  bool sgn_signOut();
  void sgn_askManagerFunc();
  void sgn_askReportForm_clicked();

private slots:
  void on_bt_signIn_clicked();
  void on_bt_back_clicked();

  void on_bt_refresh_clicked();

  void on_bt_checkReport_clicked();

private:
  Ui::ManagerWindow *ui;
  QTimer timer;
};

#endif // MANAGERWINDOW_H
