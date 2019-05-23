#include "mainwindow.h"
#include "frontdesk.h"
#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  Server server(&w);
  FrontDesk desk(&w);

  w.show();
  return a.exec();
}
