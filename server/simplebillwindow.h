#ifndef SIMPLEBILLWINDOW_H
#define SIMPLEBILLWINDOW_H

#include "../share/objects.h"

#include <QWidget>

namespace Ui {
class SimpleBillWindow;
}

class SimpleBillWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleBillWindow(QWidget *parent = nullptr);
    ~SimpleBillWindow();

    void showSimpleBill(Room room);

private:
    Ui::SimpleBillWindow *ui;
};

#endif // SIMPLEBILLWINDOW_H
