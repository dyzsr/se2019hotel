#ifndef RECORDSWINDOW_H
#define RECORDSWINDOW_H

#include <QWidget>

#include <QListWidgetItem>
#include <QFileDialog>
#include <QStringList>
#include <QString>

#include "../share/objects.h"

namespace Ui {
class RecordsWindow;
}

class RecordsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RecordsWindow(QWidget *parent = nullptr);
    ~RecordsWindow();

    void showDetailedBill(Room room, QVector<QString> data);

private slots:
    void on_bt_return_clicked();

private:
    Ui::RecordsWindow *ui;
};

#endif // RECORDSWINDOW_H
