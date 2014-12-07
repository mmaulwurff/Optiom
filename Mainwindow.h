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

private:
    Ui::MainWindow * const ui;
    QList<QSettings *> settingsList;

    static QString makeGroupName(QString groupName);
};

#endif // MAINWINDOW_H
