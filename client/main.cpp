#include "mainwindow.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  Client client(&w);

  QObject::connect(&client, &Client::sgn_refresh, &w, &MainWindow::refresh);
  QObject::connect(&w, &MainWindow::sgn_signIn, &client, &Client::signIn);
  QObject::connect(&w, &MainWindow::sgn_signOut, &client, &Client::signOut);
  QObject::connect(&w, &MainWindow::sgn_setTemp, &client, &Client::setTemp);
  QObject::connect(&w, &MainWindow::sgn_setWdspd, &client, &Client::setWdspd);
  QObject::connect(&w, &MainWindow::sgn_setState, &client, &Client::setState);
  QObject::connect(&w, &MainWindow::sgn_getIn, &client, &Client::getIn);
  QObject::connect(&client, &Client::sgn_getRecoverStr, &w, &MainWindow::slot_getRecoverStr);

  w.show();
  return a.exec();
}
