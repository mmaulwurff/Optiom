#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Optiom, " + QDir::currentPath());

    for (const QString fileName : QDir::current().entryList(QStringList("*.ini"))) {
        QVBoxLayout * const layout = new QVBoxLayout;

        QSettings * const setting = new QSettings(fileName, QSettings::IniFormat);
        settingsList.append(setting);

        const QStringList groups = QStringList("General") << setting->childGroups();
        for (const QString groupName : groups) {
            setting->endGroup();
            if (groupName != "General") setting->beginGroup(groupName);
            if (groups.size() != 1) layout->addWidget(new QLabel("<h1>" + groupName + "</h1>"));

            for (const QString key : setting->childKeys()) {
                QHBoxLayout * const lineLayout = new QHBoxLayout;

                lineLayout->addWidget(new QLabel(key));
                QLineEdit * const lineEdit = new QLineEdit(setting->value(key).toString());
                connect(lineEdit, &QLineEdit::textEdited,
                    [=](const QString &newValue) {
                    setting->endGroup();
                    setting->setValue(((groupName == "General") ? "" : (groupName + "/")) + key, newValue);
                });

                lineLayout->addWidget(lineEdit);
                layout->addLayout(lineLayout);
            }
        }

        QWidget * const tabWidget = new QWidget;
        tabWidget->setLayout(layout);
        QScrollArea * const scrollArea = new QScrollArea;
        scrollArea->setWidget(tabWidget);
        scrollArea->setWidgetResizable(true);
        ui->tabWidget->addTab(scrollArea, fileName);
    }
}

MainWindow::~MainWindow()
{
    for (auto setting : settingsList) delete setting;
    delete ui;
}
