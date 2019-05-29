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
  QObject::connect(&w, &MainWindow::sgn_sendRequest, &client, &Client::sendRequest);
  QObject::connect(&client, &Client::sgn_getRecoverStr, &w, &MainWindow::slot_getRecoverStr);
  QObject::connect(&client, &Client::sgn_init, &w, &MainWindow::init);
  QObject::connect(&client, &Client::sgn_disable, &w, &MainWindow::disable);

  w.show();
  return a.exec();
}
