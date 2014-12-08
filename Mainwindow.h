#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadTabs();
    void showAbout();
    void showAboutQt();

private:
    Ui::MainWindow * const ui;
    QList<QSettings *> settingsList;

    static QString makeGroupName(QString groupName);
    static QStringList& addFiles(QString path, QStringList& list);
};

#endif // MAINWINDOW_H
