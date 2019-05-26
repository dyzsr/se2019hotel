#include "managercontrol.h"

ManagerControl::ManagerControl(QObject *parent):
  QObject(parent),
  manager(parent),
  window(static_cast<QWidget *>(parent))
{
  connect(&window, &ManagerWindow::sgn_signIn, &manager, &Manager::signIn);
  connect(&window, &ManagerWindow::sgn_signOut, &manager, &Manager::signOut);
  window.hide();
}

void ManagerControl::openNewWindow()
{
  window.init();
  window.show();
}
