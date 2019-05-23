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

signals:
  bool sgn_signIn(QString name, QString pswd);
  bool sgn_signOut();

private slots:
  void on_bt_signIn_clicked();
  void on_bt_back_clicked();

private:
  Ui::ManagerWindow *ui;
};

#endif // MANAGERWINDOW_H
