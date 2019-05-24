#ifndef MANAGERCONTROL_H
#define MANAGERCONTROL_H

#include "manager.h"
#include "managerwindow.h"

#include <QObject>

class ManagerControl: public QObject
{
  Q_OBJECT

public:
  explicit ManagerControl(QObject *parent = nullptr);

public slots:
  void openNewWindow();

private:
  Manager manager;
  ManagerWindow window;
};

#endif // MANAGERCONTROL_H
