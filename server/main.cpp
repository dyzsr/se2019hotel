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

  QObject::connect(&w, &MainWindow::sgn_signup, &server, &Server::addUser);

  QObject::connect(&w, &MainWindow::sgn_del, &server, &Server::delUser);

  QObject::connect(&w, &MainWindow::sgn_setMode, &server, &Server::setMode);

  QObject::connect(&w, &MainWindow::sgn_checkin, &server, &Server::checkInFromServer);

  QObject::connect(&w, &MainWindow::sgn_getUsrId, &server, &Server::getUsrId);

  QObject::connect(&w, &MainWindow::sgn_getUsrIds, &server, &Server::getUsrIds);

  QObject::connect(&w, &MainWindow::sgn_getAvailUsrIds, &server, &Server::getAvailUsrIds);

  QObject::connect(&server, &Server::sgn_init, &w, &MainWindow::init);

  QObject::connect(&w, &MainWindow::sgn_checkout, &server, &Server::checkOut);

  QObject::connect(&w, &MainWindow::sgn_openNewWindow,
                   &manager_control, &ManagerControl::openNewWindow);
  QObject::connect(&w, &MainWindow::sgn_checkRoomIdValid,
                   &manager_control, &ManagerControl::slot_checkRoomIdValid);
  QObject::connect(&w, &MainWindow::sgn_askSimpleBill_clicked,
                   &manager_control, &ManagerControl::slot_askSimpleBill_clicked);
  QObject::connect(&w, &MainWindow::sgn_askDetailedBill_clicked,
                   &manager_control, &ManagerControl::slot_askDetailedBill_clicked);

  QObject::connect(&manager_control, &ManagerControl::sgn_getRoom,
                   &server, &Server::getRoom);

  RecordsControl recordsControl(&w);
  QObject::connect(&manager_control, &ManagerControl::sgn_showSimpleBill,
                   &recordsControl, &RecordsControl::slot_showSimpleBill);
  QObject::connect(&manager_control, &ManagerControl::sgn_showDetailedBill,
                   &recordsControl, &RecordsControl::slot_showDetailedBill);
  QObject::connect(&manager_control, &ManagerControl::sgn_showReportForm,
                   &recordsControl, &RecordsControl::slot_showReportForm);

  server.init();
  w.show();
  return a.exec();
}
