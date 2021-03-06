#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSettings;
class QFileSystemModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
     MainWindow();
    ~MainWindow();

private slots:
    void loadSetting(QString path);
    void showAbout();
    void showAboutQt();

private:
    Ui::MainWindow * const ui;
    QSettings * setting;
    QWidget * settingWidget;
    QFileSystemModel * const fileSystemModel;
    const QString generalGroupName;

    QString makeGroupName(QString groupName) const;
    static QStringList & addFiles(QString path, QStringList & list);

    QString getSelectedFilePath() const;
};

#endif // MAINWINDOW_H
