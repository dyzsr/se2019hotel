#include "pipe.h"

Pipe::Pipe():
  db(QSqlDatabase::database("QODBC"))
{
  db.open();
}

Pipe::~Pipe()
{
  db.close();
}
