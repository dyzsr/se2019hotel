#ifndef SIMPLEBILLWINDOW_H
#define SIMPLEBILLWINDOW_H

#include <QWidget>

#include <QListWidgetItem>
#include <QFileDialog>
#include <QStringList>
#include <QString>

#include "../share/objects.h"
namespace Ui {
class SimpleBillWindow;
}

class SimpleBillWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleBillWindow(QWidget *parent = nullptr);
    ~SimpleBillWindow();

    void showSimpleBill(Room room , QVector<QString> data);

private slots:
    void on_pushButton_back_clicked();
    void on_pushButton_print_clicked();

private:
    Ui::SimpleBillWindow *ui;
    Room printRooms;
    QVector<QString> printDatas;
    void printSimpleBill();
};

#endif // SIMPLEBILLWINDOW_H
