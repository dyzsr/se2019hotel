#include "mainwindow.h"
#include "server.h"
#include "managercontrol.h"

#include "recordscontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  Server server(&w);
  ManagerControl manager_control(&w);

  QObject::connect(&w, &MainWindow::sgn_checkout, &server, &Server::checkOut);

  QObject::connect(&w, &MainWindow::sgn_openNewWindow,
                   &manager_control, &ManagerControl::openNewWindow);

  RecordsControl recordsControl(&w);
  QObject::connect(&w, &MainWindow::sgn_getBillings,
                   &recordsControl, &RecordsControl::slot_detailedBill);

  w.show();
  return a.exec();
}
