#include "reportformwindow.h"
#include "ui_reportformwindow.h"

ReportFormWindow::ReportFormWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportFormWindow)
{
    ui->setupUi(this);
}

ReportFormWindow::~ReportFormWindow()
{
    delete ui;
}

void ReportFormWindow::showReportForm(QVector<Billing> billings)
{
    //To Do
    this->show();
}
