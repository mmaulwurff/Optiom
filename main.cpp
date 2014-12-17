#include "Mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator qtTranslator, optiomTranslator;
    optiomTranslator.load("optiom_" + QLocale::system().name());
    qtTranslator    .load("qt_"     + QLocale::system().name(),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    a.installTranslator(&optiomTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
