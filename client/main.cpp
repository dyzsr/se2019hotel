#include "mainwindow.h"
#include "client.h"
#include "reception.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  Pipe pipe;

  Client client(&w);
  Reception reception(&w);

  client.setPipe(&pipe);
  client.start();

  QObject::connect(&client, &Client::sgn_refresh, &w, &MainWindow::refresh);

  w.show();
  return a.exec();
}
