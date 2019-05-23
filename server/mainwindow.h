#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
  void on_bt_signIn_clicked();
  void on_bt_back_clicked();
  void on_bt_callManager_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
