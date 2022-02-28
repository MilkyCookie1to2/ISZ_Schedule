#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <iostream>
#include <string>
#include "BasicExcel.hpp"
#include <QTableWidget>
#include <QtQuick>
#include <QDate>

using namespace YExcel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->quickWidget->setHidden(true);
    ui->quickWidget->setSource(QUrl("qrc:/swipeqml.qml"));
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    data_schedule = new QQuickWidget(this);
    data_schedule->setSource(QUrl("qrc:/schedule.qml"));
    data_schedule->setResizeMode(QQuickWidget::SizeRootObjectToView);

    QPixmap pixmap1(":/previous.png");
    QIcon ButtonIcon1(pixmap1);
    ui->pushButton_4->setIcon(ButtonIcon1);
    ui->pushButton_4->setIconSize(QSize(35,35));

    QPixmap pixmap2(":/next.png");
    QIcon ButtonIcon2(pixmap2);
    ui->pushButton_5->setIcon(ButtonIcon2);
    ui->pushButton_5->setIconSize(QSize(35,35));

    ui->pushButton_3->setText("");
    QPixmap pixmap(":/PngItem_3900549.png");
    QIcon ButtonIcon(pixmap);
    ui->pushButton_3->setIcon(ButtonIcon);
    ui->pushButton_3->setIconSize(QSize(35,35));

    data_schedule->setHidden(true);
    ui->pushButton_2->setHidden(true);
    ui->pushButton_3->setHidden(true);
    ui->pushButton_4->setHidden(true);
    ui->pushButton_5->setHidden(true);

    QFile file(":/MaterialDark.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString css = in.readAll();
    qApp->setStyleSheet(css);

    ui->comboBoxSelFKL->addItem("Факультет");
    ui->comboBoxSelFKL->addItem("Искусств");
    ui->comboBoxSelFKL->addItem("Гуманитарный");
    ui->comboBoxSelKURS->setHidden(true);
    ui->comboBoxFOB->setHidden(true);
    ui->comboBoxSelSPL->setHidden(true);
    ui->comboBoxSelGR->setHidden(true);

    loadind=new QProgressIndicator(this);

    // Initialize manager ...
    manager = new QNetworkAccessManager();
    // ... and connect the signal to the handler
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
    connect(manager, &QNetworkAccessManager::sslErrors, this, &MainWindow::sslErrors);

    ui->pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->pushButton_3->setMinimumHeight(ui->pushButton_3->height()*2);
    ui->pushButton_3->setMaximumWidth(ui->pushButton_3->height());
    ui->pushButton_2->setMinimumHeight(ui->pushButton_2->height()*2);
    ui->pushButton_4->setMinimumHeight(ui->pushButton_4->height()*2);
    ui->pushButton_5->setMinimumHeight(ui->pushButton_5->height()*2);
    ui->pushButton_4->setMaximumWidth(ui->pushButton_4->height());
    ui->pushButton_5->setMaximumWidth(ui->pushButton_5->height());
    ui->pushButton->setMinimumHeight(ui->pushButton->height()*2);
    ui->comboBoxFOB->setMinimumHeight(ui->comboBoxFOB->height()*1.5);
    ui->comboBoxSelSPL->setMinimumHeight(ui->comboBoxSelSPL->height()*1.5);
    ui->comboBoxSelFKL->setMinimumHeight(ui->comboBoxSelFKL->height()*1.5);
    ui->comboBoxSelKURS->setMinimumHeight(ui->comboBoxSelKURS->height()*1.5);
    ui->comboBoxSelGR->setMinimumHeight(ui->comboBoxSelGR->height()*1.5);

    start_app();
    QDir dir;
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      QString str = fileInfo.absolutePath();
      QFile file(fileInfo.fileName());
      //file.remove();
      qDebug() << str;
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save_settings()
{
    QFile *file = new QFile("Settings");
    if(file->open(QFile::WriteOnly)){
        file->write(filenamerasp.toUtf8());
        file->write("\n");
        file->write(QString::number(number_group).toUtf8());
        file->write("\n");
        file->write(specl.toUtf8());
        file->write("\n");
        file->write(form_obch.toUtf8());
        file->write("\n");
        file->write(kurs.toUtf8());
        file->close();
    }
}

void MainWindow::start_app()
{   
    QFile set("Settings");
    if(set.exists())
    {
        set.open(QIODevice::ReadOnly);
        QTextStream in(&set);
        int i=0;
        while (!in.atEnd()&&i!=4) {
            switch(i)
            {
            case 0: {filenamerasp=in.readLine();break;}
            case 1: {number_group=in.readLine().toInt();break;}
            case 2: {specl = in.readLine();break;}
            case 3: {form_obch = in.readLine();break;}
            }
            i++;
        }
        if(!filenamerasp.isEmpty() && number_group && !specl.isEmpty() && !form_obch.isEmpty())
        {
            QFile rasp(filenamerasp);
            if(rasp.exists())
            {
                ui->label->setHidden(true);
                ui->comboBoxSelFKL->setHidden(true);
                ui->verticalSpacer->changeSize(40,0);
                ui->quickWidget->setHidden(false);
                if(filenamerasp[filenamerasp.length()-1]=='s')
                    read_xls();
                else
                    read_xlsm();
                QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
                QMetaObject::invokeMethod(obj, "remove_funtom");
                QMetaObject::invokeMethod(obj, "setcurindex");
                ui->pushButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
                ui->pushButton->setMaximumWidth(ui->pushButton->height());
                //ui->pushButton->setText("Выйти");
                ui->pushButton->setText("");
                QPixmap pixmap(":/exit-icon-png-23.jpeg");
                QIcon ButtonIcon(pixmap);
                ui->pushButton->setIcon(ButtonIcon);
                ui->pushButton->setIconSize(QSize(35,35));
                ui->pushButton_2->setHidden(false);
                ui->pushButton_3->setHidden(false);
                ui->pushButton_4->setHidden(false);
                ui->pushButton_5->setHidden(false);
            }
        }
    }
}

void MainWindow::read_xlsm()
{
    QStringList list_of_day;
    Document doc(filenamerasp);
    if (!doc.load())
        qDebug() << "oh no. it is not read";
    for(QString currentSheetName: doc.sheetNames())
    {
        QDate data(1899,12,30);
        doc.selectSheet(currentSheetName);
        int maxRow = -1;
        int maxCol = -1;
        doc.currentWorksheet()->getFullCells(&maxRow, &maxCol);
        bool found_date=false;
        int day = 1;
        for(int row=1;row<maxRow;row++)
        {
            QVariant var = doc.read( row, 2 );
            if(var.typeId() == 10)
            {
                QVariant var1 = doc.read( row, 1 );
                if(found_date==false)
                {
                    if(data != QDate(1899,12,30))
                    {
                        data = data.addDays(1);
                        list_of_day.push_back(data.toString("dd.MM.yyyy"));
                    }
                    else
                    {
                        if(!var1.toDate().isNull())
                        {
                            data = var1.toDate();
                            list_of_day.push_back(data.toString("dd.MM.yyyy"));
                        }
                        else
                        {
                            var1 = doc.read( row-1, 1 );
                            if(!var1.toDate().isNull())
                            {
                                data = var1.toDate();
                                list_of_day.push_back(data.toString("dd.MM.yyyy"));
                            }
                            else
                            {
                                var1=doc.read(row+3,1);
                                if(!var1.toDate().isNull())
                                {
                                    data = var1.toDate();
                                    list_of_day.push_back(data.toString("dd.MM.yyyy"));
                                }
                            }
                        }
                    }
                    for(int i=row+1;i<maxRow;i++)
                    {
                        var1 = doc.read( i, 1 );
                        if(var1.typeId() == 10)
                        {
                            list_of_day.push_back(var1.toString().remove('-'));
                            found_date=true;
                            break;
                        }
                    }
                }
                QString time = var.toString();
                if(time.length()==8)
                {
                    time.insert(1,':');
                    time.insert(7,':');
                }
                else
                {
                    time.insert(2,':');
                    time.insert(8,':');
                }
                list_of_day.push_back(time);//time
                int j;
                for(j=number_group+2;j!=2;j--)
                {
                    var = doc.read( row, j );
                    if(var.typeId() == 10)
                    {
                        list_of_day.push_back("");
                        for(int z=row;z<row+3;z++)
                        {
                            var = doc.read( z, j );
                            if(!var.toString().isEmpty())
                                list_of_day[list_of_day.length()-1] += (var.toString() +" ");
                        }
                        break;
                    }
                    if(doc.cellAt(row,j)->format().leftBorderStyle() || doc.cellAt(row,j-1)->format().rightBorderStyle())
                        break;
                }
                if(j==2 || doc.read( row, j ).toString().isEmpty())
                    list_of_day.pop_back();
                var = doc.read( row+3, 2 );
                if(var.typeId() == 0)
                {
                    sent(list_of_day);
                    found_date=false;
                    list_of_day.clear();
                    day++;
                    if(day==7)
                    {
                        list_of_day.push_back(data.addDays(1).toString("dd.MM.yyyy"));
                        list_of_day.push_back("ВОСКРЕСЕНЬЕ");
                        sent(list_of_day);
                        list_of_day.clear();
                        break;
                    }
                }
            }
        }

    }
}

void MainWindow::read_xls()
{
    QStringList list_of_day;
    char *name = new char[filenamerasp.toStdString().length()];
        for(int i=0;i<filenamerasp.toStdString().length();i++)
            name[i]=filenamerasp.toStdString()[i];
    BasicExcel e;
    if (e.Load(name))
        qDebug() << "it's work";
    for(size_t sheet_number = 0; sheet_number < e.GetTotalWorkSheets(); sheet_number++)
    {
        QDate data(1899,12,30);
        BasicExcelWorksheet* sheet = e.GetWorksheet(sheet_number);
        if(sheet)
        {
            bool found_date=false;
            int day =1;
            for(size_t row=0;row<sheet->GetTotalRows();row++)
            {
                BasicExcelCell* cell = sheet->Cell(row,1);
                if(cell->Type()== BasicExcelCell::STRING)
                {
                    BasicExcelCell* cell1 = sheet->Cell(row,0);
                    if(found_date==false)
                    {
                        if(data != QDate(1899,12,30))
                        {
                            data = data.addDays(1);
                            list_of_day.push_back(data.toString("dd.MM.yyyy"));
                        }
                        else
                        {
                            if(cell1->Type()==BasicExcelCell::DOUBLE)
                            {
                                data = data.addDays(cell1->GetDouble());
                                list_of_day.push_back(data.toString("dd.MM.yyyy"));
                            }
                            else
                            {
                                cell1 = sheet->Cell(row-1,0);
                                if(cell1->Type()==BasicExcelCell::DOUBLE)
                                {
                                    data = data.addDays(cell1->GetDouble());
                                    list_of_day.push_back(data.toString("dd.MM.yyyy"));
                                }
                            }
                        }
                        for(size_t i=row+1;i<sheet->GetTotalRows();i++)
                        {
                            cell1 = sheet->Cell(i,0);
                            if(cell1->Type()==BasicExcelCell::WSTRING)
                            {
                                list_of_day.push_back(QString::fromStdWString(cell1->GetWString()).remove('-'));
                                found_date=true;
                                break;
                            }
                        }
                    }
                    QString time = QString::fromStdString(cell->GetString());
                    if(time.length()==8)
                    {
                        time.insert(1,':');
                        time.insert(7,':');
                    }
                    else
                    {
                        time.insert(2,':');
                        time.insert(8,':');
                    }
                    list_of_day.push_back(time);//time
                    size_t j;
                    for(j=number_group+1;j!=1;j--)
                    {
                        cell = sheet->Cell(row,j);
                        if(cell->Type()!=BasicExcelCell::UNDEFINED)
                        {
                            list_of_day.push_back("");
                            for(size_t z=row;z<row+3;z++)
                            {
                                cell=sheet->Cell(z,j);
                                if(cell->Type()==BasicExcelCell::WSTRING)
                                    list_of_day[list_of_day.length()-1] += (QString::fromStdWString(cell->GetWString())+" ");
                            }
                            break;
                        }
                    }
                    if(j==1)
                        list_of_day.pop_back();
                    cell=sheet->Cell(row+3,1);
                    if(cell->Type()==BasicExcelCell::UNDEFINED)
                    {
                        sent(list_of_day);
                        found_date=false;
                        list_of_day.clear();
                        day++;
                        if(day==7)
                        {
                            list_of_day.push_back(data.addDays(1).toString("dd.MM.yyyy"));
                            list_of_day.push_back("ВОСКРЕСЕНЬЕ");
                            sent(list_of_day);
                            list_of_day.clear();
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::sent(QStringList list_day)
{
    QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
    Schedule test;
    test.for_width = qApp->screens()[0]->size().width()-12;
    test.list_day = list_day;
    ui->quickWidget->rootContext()->setContextProperty("schedule",&test);
    data_schedule->rootContext()->setContextProperty("schedule",&test);
    QMetaObject::invokeMethod(obj, "addPageSignal");
    /*qDebug() << "yeah!!!" << list_day.length();
    if(list_day.length()==1)
    {
        qDebug() << "Выходной";
        return;
    }
    qDebug() << list_day[1] << list_day[0] << "\n";
    for(int i = 2; i < list_day.length(); i++)
    {
        qDebug() << list_day[i];
    }
    qDebug()<<"\n";*/
}

void MainWindow::step2search()
{
    ui->comboBoxSelFKL->setHidden(true);
    ui->comboBoxSelSPL->setHidden(true);
    ui->comboBoxFOB->setHidden(true);
    ui->comboBoxSelKURS->setHidden(true);
    ui->comboBoxSelGR->setHidden(false);
    ui->pushButton->setText("Вывести");
    ui->comboBoxSelGR->addItem("Подгруппа");
    if(filenamerasp[filenamerasp.length()-1] == 's')
    {
        char *name = new char[filenamerasp.toStdString().length()];
            for(int i=0;i<filenamerasp.toStdString().length();i++)
                name[i]=filenamerasp.toStdString()[i];
        BasicExcel e;
        if (e.Load(name))
            qDebug() << "it's work";
        BasicExcelWorksheet* sheet1 = e.GetWorksheet(1);
        size_t maxRows = sheet1->GetTotalRows();
        size_t maxCols = sheet1->GetTotalCols();
        qDebug() << maxRows << " " << maxCols;
        if(sheet1)
        {
            for(size_t i = 0;;i++)
            {
                BasicExcelCell* cell = sheet1->Cell(i,2);
                if(cell->Type() == BasicExcelCell::WSTRING)
                {
                    QString s = QString::fromStdWString(cell->GetWString());
                    qDebug()<<s;
                    if(s.contains("ПОДГР") || s.contains("ПОДГРУППА") || s.contains("Подгруппа") ||
                            s.contains("ПРОДЮСЕРСТВО") || s.contains("ПЕНИЕ") || s.contains("КОМПЬЮТЕРНАЯ МУЗЫКА"))
                    {
                        ui->comboBoxSelGR->addItem(s);
                        for(size_t j=3;;j++)
                        {
                            BasicExcelCell* cell1 = sheet1->Cell(i,j);
                            if(cell1->Type()== BasicExcelCell::UNDEFINED)
                                break;
                            if(cell1->Type()== BasicExcelCell::WSTRING)
                            {
                                s = QString::fromStdWString(cell1->GetWString());
                                ui->comboBoxSelGR->addItem(s);
                            }
                        }
                        break;
                    }
                }
            }
            /*qDebug() << "vdvdvssvs\n\n";
            //for (size_t r=0; r<maxRows; ++r)
            {
                //for (size_t c=0; c<maxCols; ++c)
                {
                    qDebug() << "MOMENT";
                    BasicExcelCell* cell = sheet1->Cell(10,0);
                    switch (cell->Type())
                    {
                      case BasicExcelCell::UNDEFINED:
                        qDebug() << "nihuya "<<" ";
                        break;

                      case BasicExcelCell::INT:
                        qDebug() << "this is int  " <<cell->GetInteger();
                        break;

                      case BasicExcelCell::DOUBLE:
                        qDebug() <<  "this is double" <<cell->GetDouble();
                        break;

                      case BasicExcelCell::STRING:
                        qDebug() << "string" << cell->GetString();
                        break;

                      case BasicExcelCell::WSTRING:
                        qDebug() << "\nпонимаю";
                        qDebug() <<  cell->GetWString();
                        QString s = QString::fromStdWString(cell->GetWString());
                        qDebug() << s;
                        break;
                    }
                }
            }*/
        }
        delete[] name;
    }
    else
    {
        Document doc(filenamerasp);
        if (!doc.load())
            qDebug() << "oh no. it is not read";
        for(int i=1;;i++)
        {
            QVariant var = doc.read( i, 3 );
            if(var.toString().contains("ПОДГР") || var.toString().contains("ПОДГРУППА"))
            {
                ui->comboBoxSelGR->addItem(var.toString());
                for(int j=4;;j++)
                {
                    QVariant var1 = doc.read( i, j );
                    if(var1.toString()!=var.toString())
                    {
                        if(var1.toString()=="")
                            break;
                        else
                        {
                            ui->comboBoxSelGR->addItem(var1.toString());
                            var=var1;
                        }
                    }
                }
                break;
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="Найти расписание")
    {
        if(ui->comboBoxSelFKL->currentIndex()!=0 && ui->comboBoxSelSPL->currentIndex()!=0 && ui->comboBoxFOB->currentIndex()!=0 &&
                ui->comboBoxSelKURS->currentIndex()!=0)
        {
            ui->comboBoxSelFKL->setEnabled(false);
            ui->comboBoxSelSPL->setEnabled(false);
            ui->comboBoxFOB->setEnabled(false);
            ui->comboBoxSelKURS->setEnabled(false);
            loadind->setGeometry(ui->pushButton->geometry().x(),ui->pushButton->geometry().y(),ui->pushButton->size().width(),ui->pushButton->size().height());
            ui->pushButton->setHidden(true);
            loadind->setAnimationDelay(50);
            loadind->startAnimation();
            form_obch = ui->comboBoxFOB->currentText();
            specl = ui->comboBoxSelSPL->currentText();
            kurs = ui->comboBoxSelKURS->currentText();
            texturlsite = "http://isz.minsk.by/student/raspisanie-zanyatiy/raspisanie-zanyatiy.php";
            getData(texturlsite);
            return;
        }
        else
        {
            QMessageBox messageBox;
            messageBox.warning(0,"Ошибка ввода","Выберите все пункты");
            return;
        }
    }
    if(ui->pushButton->text()=="Вывести")
    {
        if(ui->comboBoxSelGR->currentIndex()!=0)
        {
            ui->comboBoxSelGR->setHidden(true);
            ui->label->setHidden(true);
            ui->verticalSpacer->changeSize(40,0);
            ui->quickWidget->setHidden(false);
            number_group = ui->comboBoxSelGR->currentIndex();
            save_settings();
            if(filenamerasp[filenamerasp.length()-1]=='s')
                read_xls();
            else
                read_xlsm();
            QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
            QMetaObject::invokeMethod(obj, "remove_funtom");
            QMetaObject::invokeMethod(obj, "setcurindex");
            ui->pushButton_2->setHidden(false);
            ui->pushButton_3->setHidden(false);
            ui->pushButton_4->setHidden(false);
            ui->pushButton_5->setHidden(false);
            ui->pushButton->setText("");
            QPixmap pixmap(":/exit-icon-png-23.jpeg");
            QIcon ButtonIcon(pixmap);
            ui->pushButton->setIcon(ButtonIcon);
            ui->pushButton->setIconSize(QSize(35,35));
            //ui->pushButton->setText("Выйти");
            ui->pushButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
            ui->pushButton->setMaximumWidth(ui->pushButton->height());
            return;
        }
        else
        {
            QMessageBox messageBox;
            messageBox.warning(0,"Ошибка ввода","Выберите подгруппу");
            return;
        }
    }
    if(ui->pushButton->text()=="")
    {
        QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
        QMetaObject::invokeMethod(obj, "delete_all");
        ui->pushButton->setMaximumWidth(16777215);
        ui->pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        ui->pushButton_2->setHidden(true);
        ui->pushButton_3->setHidden(true);
        ui->pushButton_4->setHidden(true);
        ui->pushButton_5->setHidden(true);
        ui->pushButton->setText("Найти расписание");
        ui->pushButton->setIcon(QIcon());
        ui->label->setHidden(false);
        ui->comboBoxSelFKL->setHidden(false);
        ui->comboBoxSelFKL->setCurrentIndex(0);
        ui->quickWidget->setHidden(true);
        ui->comboBoxSelFKL->setEnabled(true);
        ui->comboBoxSelSPL->setEnabled(true);
        ui->comboBoxFOB->setEnabled(true);
        ui->comboBoxSelKURS->setEnabled(true);
        ui->comboBoxSelSPL->clear();
        ui->comboBoxFOB->clear();
        ui->comboBoxSelKURS->clear();
        ui->comboBoxSelGR->clear();
        ui->comboBoxSelSPL->setHidden(true);
        ui->comboBoxFOB->setHidden(true);
        ui->comboBoxSelKURS->setHidden(true);
        ui->comboBoxSelGR->setHidden(true);
        ui->verticalSpacer->changeSize(40,20,QSizePolicy::Expanding,QSizePolicy::Expanding);
        QDir dir;
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
          QFileInfo fileInfo = list.at(i);
          QFile file(fileInfo.fileName());
          file.remove();
        }
        texturlsite.clear();
        filenamerasp.clear();
    }
}

void MainWindow::getRasp()
{
    QFile file("raspisanie-zanyatiy.php");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString linewithurl;
    QString for_search = specl;
    if(form_obch=="Заочная" && for_search == "Культурология")
        for_search = "Культурология (по направлениям)";
    while (!in.atEnd()) {
      QString line = in.readLine();
      if(line.contains(for_search))
      {
          for(int i=0; i<=kurs.toInt();i++)
              linewithurl = in.readLine();
          break;
      }
    }
    linewithurl.remove(" <a href=\"");
    QString forremove = "\">";
    forremove += kurs;
    forremove += "-й курс</a><br>";
    linewithurl.remove(forremove);
    texturlsite = "http://isz.minsk.by";
    texturlsite += linewithurl;
    qDebug()<<texturlsite;
    getData(texturlsite);
}

void MainWindow::getData(QString texturl)
{
    QUrl url(texturl);
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
}

void MainWindow::onResult(QNetworkReply *reply)
{
    // If an error occurs in the process of obtaining data
    if(reply->error()){
        // We inform about it and show the error information
        qDebug() << "ERROR";
        qDebug() << reply->errorString();
        QMessageBox messageBox;
        messageBox.critical(0,"Error",reply->errorString());
        loadind->stopAnimation();
        loadind->setGeometry(0,0,loadind->size().width(),loadind->size().height());
        ui->pushButton->setHidden(false);
        ui->comboBoxSelFKL->setEnabled(true);
        ui->comboBoxSelSPL->setEnabled(true);
        ui->comboBoxFOB->setEnabled(true);
        ui->comboBoxSelKURS->setEnabled(true);
    } else {
        // Otherwise we create an object file for use with
        QUrl url(texturlsite);
        QFileInfo fileInfo(url.path());
        QString fileName=fileInfo.fileName();
        qDebug() << fileName << " if file ";
        QFile *file = new QFile(fileName);
        // Create a file, or open it to overwrite ...
        if(file->open(QFile::WriteOnly)){
            file->write(reply->readAll());  // ... and write all the information from the page file
            file->close();                  // close file
        qDebug() << "Downloading is completed";
        if(fileName=="raspisanie-zanyatiy.php")
            getRasp();
        else
        {
            filenamerasp = fileName;
            loadind->stopAnimation();
            loadind->setGeometry(0,0,loadind->size().width(),loadind->size().height());
            ui->pushButton->setHidden(false);
            step2search();
        }
        emit onReady(); // Sends a signal to the completion of the receipt of the file
        }
    }
}

void MainWindow::sslErrors(QNetworkReply *reply)
{
    reply->ignoreSslErrors();
}

void MainWindow::on_comboBoxSelFKL_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Факультет")
    {
        ui->comboBoxSelKURS->setHidden(true);
        ui->comboBoxSelKURS->clear();
        ui->comboBoxFOB->setHidden(true);
        ui->comboBoxFOB->clear();
        ui->comboBoxSelSPL->setHidden(true);
        ui->comboBoxSelSPL->clear();
        ui->comboBoxSelGR->setHidden(true);
        ui->comboBoxSelGR->clear();
    }
    if(arg1 == "Искусств")
    {
        ui->comboBoxSelSPL->clear();
        ui->comboBoxSelSPL->setHidden(false);
        ui->comboBoxSelSPL->addItem("Специальность");
        ui->comboBoxSelSPL->addItem("Дизайн (виртуальной среды)");
        ui->comboBoxSelSPL->addItem("Дизайн (предметно-пространственной среды)");
        ui->comboBoxSelSPL->addItem("Дизайн (костюма и тканей)");
        ui->comboBoxSelSPL->addItem("Искусство эстрады");
    }
    if(arg1 == "Гуманитарный")
    {
        ui->comboBoxSelSPL->clear();
        ui->comboBoxSelSPL->setHidden(false);
        ui->comboBoxSelSPL->addItem("Специальность");
        ui->comboBoxSelSPL->addItem("Культурология");
        ui->comboBoxSelSPL->addItem("Лингвистическое обеспечение межкультурных коммуникаций");
    }
}


void MainWindow::on_comboBoxFOB_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Форма обучения")
    {
        ui->comboBoxSelKURS->setHidden(true);
        ui->comboBoxSelKURS->clear();
        ui->comboBoxSelGR->setHidden(true);
        ui->comboBoxSelGR->clear();
    }
    if(arg1 == "Дневная")
    {
        ui->comboBoxSelKURS->clear();
        ui->comboBoxSelKURS->setHidden(false);
        ui->comboBoxSelKURS->addItem("Курс");
        ui->comboBoxSelKURS->addItem("1");
        ui->comboBoxSelKURS->addItem("2");
        ui->comboBoxSelKURS->addItem("3");
        ui->comboBoxSelKURS->addItem("4");
        if(ui->comboBoxSelSPL->currentText()!="Искусство эстрады" &&
                ui->comboBoxSelSPL->currentText()!="Культурология")
        {
            ui->comboBoxSelKURS->addItem("5");
        }
    }
    if(arg1 == "Заочная")
    {
        ui->comboBoxSelKURS->clear();
        ui->comboBoxSelKURS->setHidden(false);
        ui->comboBoxSelKURS->addItem("Курс");
        ui->comboBoxSelKURS->addItem("1");
        ui->comboBoxSelKURS->addItem("2");
        ui->comboBoxSelKURS->addItem("3");
        ui->comboBoxSelKURS->addItem("4");
        ui->comboBoxSelKURS->addItem("5");
    }
}


void MainWindow::on_comboBoxSelSPL_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Специальность")
    {
        ui->comboBoxSelKURS->setHidden(true);
        ui->comboBoxSelKURS->clear();
        ui->comboBoxFOB->setHidden(true);
        ui->comboBoxFOB->clear();
        ui->comboBoxSelGR->setHidden(true);
        ui->comboBoxSelGR->clear();
    }
    if(arg1 == "Дизайн (виртуальной среды)" || arg1 == "Дизайн (предметно-пространственной среды)" ||
            arg1 == "Дизайн (костюма и тканей)" || arg1 == "Искусство эстрады" ||
            arg1 == "Лингвистическое обеспечение межкультурных коммуникаций")
    {
        ui->comboBoxFOB->clear();
        ui->comboBoxFOB->setHidden(false);
        ui->comboBoxFOB->addItem("Форма обучения");
        ui->comboBoxFOB->addItem("Дневная");
    }
    if(arg1 == "Культурология")
    {
        ui->comboBoxFOB->clear();
        ui->comboBoxFOB->setHidden(false);
        ui->comboBoxFOB->addItem("Форма обучения");
        ui->comboBoxFOB->addItem("Дневная");
        ui->comboBoxFOB->addItem("Заочная");
    }
}


void MainWindow::on_comboBoxSelKURS_currentTextChanged(const QString &arg1)
{
    if(arg1=="Курс")
    {
        ui->comboBoxSelGR->setHidden(true);
        ui->comboBoxSelGR->clear();
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
    QMetaObject::invokeMethod(obj, "setcurindex");
}


void MainWindow::on_pushButton_5_clicked()
{
    QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
    QMetaObject::invokeMethod(obj, "next_page");
}


void MainWindow::on_pushButton_4_clicked()
{
    QObject *obj = ui->quickWidget->rootObject()->findChild<QObject *>("swipeview");
    QMetaObject::invokeMethod(obj, "previous_page");
}


