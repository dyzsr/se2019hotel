#ifndef RECEPTION_H
#define RECEPTION_H

#include "../share/objects.h"

#include <QObject>
#include <QVector>

class Reception : public QObject
{
  Q_OBJECT
public:
  explicit Reception(QObject *parent = nullptr);

signals:
  void sgn_printBilling(QVector<Billing> billings);

public slots:
  void getBillings();

private:
  QVector<Billing> billings;
};

#endif // RECEPTION_H
