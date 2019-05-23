#include "frontdesk.h"

#include <QDebug>

FrontDesk::FrontDesk(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  Q_ASSERT(pipe != nullptr);
}
