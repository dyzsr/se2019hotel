#include "mainwindow.h"
#include "manager.h"
#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  Server server(&w);
  Manager manager(&w);

  w.show();
  return a.exec();
}
