#include "mainwindow.h"
#include "frontdesk.h"
#include "server.h"
#include "managercontrol.h"

#include "recordscontrol.h"

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
  QObject::connect(&desk, &FrontDesk::sgn_checkout, &server, &Server::checkOut);

  QObject::connect(&w, &MainWindow::sgn_openNewWindow,
                   &manager_control, &ManagerControl::openNewWindow);

  RecordsControl recordsControl;
  QObject::connect(&w, &MainWindow::sgn_getBillings,
                   &recordsControl, &RecordsControl::slot_detailedBill);

  w.show();
  return a.exec();
}
