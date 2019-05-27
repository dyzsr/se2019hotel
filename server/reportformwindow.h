#ifndef REPORTFORMWINDOW_H
#define REPORTFORMWINDOW_H

#include <QWidget>

#include "../share/objects.h"

namespace Ui {
class ReportFormWindow;
}

class ReportFormWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReportFormWindow(QWidget *parent = nullptr);
    ~ReportFormWindow();

    void showReportForm(QVector<Billing> billings);

private:
    Ui::ReportFormWindow *ui;
};

#endif // REPORTFORMWINDOW_H
