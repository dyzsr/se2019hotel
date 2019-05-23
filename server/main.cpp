#include "mainwindow.h"
#include "frontdesk.h"
#include "server.h"
#include "managercontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  Server server(&w);
  FrontDesk desk(&w);
  ManagerControl manager_control(&w);

  QObject::connect(&w, &MainWindow::sgn_checkout, &desk, &FrontDesk::checkOut);
  QObject::connect(&w, &MainWindow::sgn_getBillings, &desk, &FrontDesk::getBillings);

  QObject::connect(&w, &MainWindow::sgn_openNewWindow,
                   &manager_control, &ManagerControl::openNewWindow);

  w.show();
  return a.exec();
}
