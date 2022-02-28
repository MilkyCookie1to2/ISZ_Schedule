#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork>
#include <QFile>
#include <QDir>
#include <QSslSocket>
#include <QProgressIndicator>
#include <QMessageBox>
#include <QQuickWidget>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

#include "schedule.h"

using namespace QXlsx;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    friend class Schedule;

signals:
    void onReady();
    void doAddPage(QStringList);

private slots:
    void on_pushButton_clicked();
    void onResult(QNetworkReply *);
    void getData(QString);

    void sslErrors(QNetworkReply *);
    void getRasp();

    void on_comboBoxSelFKL_currentTextChanged(const QString &arg1);

    void on_comboBoxFOB_currentTextChanged(const QString &arg1);

    void on_comboBoxSelSPL_currentTextChanged(const QString &arg1);

    void step2search();

    void on_comboBoxSelKURS_currentTextChanged(const QString &arg1);

    void read_xls();

    void read_xlsm();

    void sent(QStringList);

    void on_pushButton_2_clicked();

    void start_app();

    void save_settings();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QString texturlsite;
    QProgressIndicator *loadind;
    QString filenamerasp;
    int number_group;
    QString form_obch;
    QString specl;
    QString kurs;
    QQuickWidget*m_quickWidget;
    QQuickWidget *data_schedule;
};


#endif // MAINWINDOW_H
