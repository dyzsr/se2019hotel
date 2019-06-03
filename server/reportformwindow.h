#ifndef REPORTFORMWINDOW_H
#define REPORTFORMWINDOW_H

#include <QWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QStringList>
#include <QString>
#include <QtWidgets/QMainWindow>
#include "records.h"
#include "../share/objects.h"
#include "../share/pipe.h"
namespace Ui {
class ReportFormWindow;
}

class ReportFormWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReportFormWindow(QWidget *parent = nullptr, Records *rec = nullptr);
    ~ReportFormWindow();

    void showReportForm(QVector<QString> data);

    void Updatereport(QVector<QString> data);

public slots:
    void on_OK_clicked();

private slots:

    void on_go_back_clicked();

    void on_print_clicked();

    void on_viewreport_clicked();

    void on_viewfee_clicked();

private:
    Records *records;
    Ui::ReportFormWindow *ui;
    Room printRoo;
    int rrr;
    QVector<QString> printDat;
    QMainWindow window;
};

#endif // REPORTFORMWINDOW_H
