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

    void showSimpleBill(QVector<QString> data);
    void showDetailedBill(Room room, QVector<QString> data);
    void showReportForm(QVector<QString> data);

private slots:
    void on_bt_return_clicked();

private:
    Ui::RecordsWindow *ui;
    QVector<QString> printData;
    QString recordTitle;
    void hideOptionalItems();
};

#endif // RECORDSWINDOW_H
