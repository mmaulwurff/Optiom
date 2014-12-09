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
    void loadSetting(QString path);
    void showAbout();
    void showAboutQt();

private:
    Ui::MainWindow * const ui;
    QSettings * setting;
    QWidget * settingWidget;

    static QString makeGroupName(QString groupName);
    static QStringList& addFiles(QString path, QStringList& list);
};

#endif // MAINWINDOW_H
