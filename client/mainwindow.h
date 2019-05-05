#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
  PipeClient pipe;
};

#endif // MAINWINDOW_H
