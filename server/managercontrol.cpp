#include "managercontrol.h"

ManagerControl::ManagerControl(QObject *parent):
  QObject(parent)
{
}

void ManagerControl::openNewWindow(QWidget *parent)
{
  if (window == nullptr) {
    window = new ManagerWindow(parent);
    manager = new Manager(window);
    window->show();
    init();
  } else if (!window->isVisible()) {
    window->show();
  }
}

void ManagerControl::init()
{
  connect(window, &ManagerWindow::sgn_signIn, manager, &Manager::signIn);
  connect(window, &ManagerWindow::sgn_signOut, manager, &Manager::signOut);
}
