#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "pipeclient.h"

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

private slots:
  void on_checkInButton_clicked();

private:
  Ui::MainWindow *ui;
  Client client;
};

#endif // MAINWINDOW_H
