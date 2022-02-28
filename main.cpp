#include "mainwindow.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /*QGuiApplication a(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/swipesrc.qml"));*/
    return a.exec();
}
