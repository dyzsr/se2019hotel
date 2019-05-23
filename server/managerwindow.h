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

private:
  Ui::ManagerWindow *ui;
};

#endif // MANAGERWINDOW_H
