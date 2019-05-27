#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QWidget>

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

private slots:
  void on_bt_signIn_clicked();
  void on_bt_back_clicked();

  void on_bt_refresh_clicked();

private:
  Ui::ManagerWindow *ui;

};

#endif // MANAGERWINDOW_H
