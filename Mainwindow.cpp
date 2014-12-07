#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget * const parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QDir::currentPath());
    loadTabs();

    ui->actionReload->setShortcut(QKeySequence::Refresh);
    connect(ui->actionReload, &QAction::triggered, this, &MainWindow::loadTabs);
    connect(ui->actionClose,  &QAction::triggered, &QApplication::quit);
}

void MainWindow::loadTabs() {
    ui->tabWidget->clear();

    for (const QString fileName : QDir::current().entryList(QStringList("*.ini"))) {
        QVBoxLayout * const layout = new QVBoxLayout;

        QSettings * const setting = new QSettings(fileName, QSettings::IniFormat);
        settingsList.append(setting);

        const QStringList groups = QStringList("General") << setting->childGroups();
        for (const QString groupName : groups) {
            if (groupName != "General") setting->beginGroup(groupName);
            if (groups.size() != 1) layout->addWidget(new QLabel("<h1>" + groupName + "</h1>"));

            for (const QString key : setting->childKeys()) {
                QHBoxLayout * const lineLayout = new QHBoxLayout;

                lineLayout->addWidget(new QLabel(key));

                const QString type = key.left(3);
                if (type == "num" || type == "per") {
                    QSpinBox * const spinBox = new QSpinBox;
                    spinBox->setValue(setting->value(key).toInt());
                    spinBox->setMaximum((type == "per") ? 100 : std::numeric_limits<int>::max());
                    connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                        [=](const int newValue) {
                        setting->setValue(makeGroupName(groupName) + key, newValue);
                    });
                    lineLayout->addWidget(spinBox);
                } else if (type == "is_") {
                    QCheckBox * const checkBox = new QCheckBox;
                    checkBox->setChecked(setting->value(key).toBool());
                    connect(checkBox, &QCheckBox::stateChanged,
                        [=](const int state) {
                        setting->setValue(makeGroupName(groupName) + key, (state == Qt::Checked));
                    });
                    lineLayout->addWidget(checkBox);
                } else {
                    QLineEdit * const lineEdit = new QLineEdit(setting->value(key).toString());
                    lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
                    connect(lineEdit, &QLineEdit::textEdited,
                        [=](const QString &newValue) {
                        setting->setValue(makeGroupName(groupName) + key, newValue);
                    });
                    lineLayout->addWidget(lineEdit);
                }

                layout->addLayout(lineLayout);
            }
            setting->endGroup();
        }

        QWidget * const tabWidget = new QWidget;
        tabWidget->setLayout(layout);
        QScrollArea * const scrollArea = new QScrollArea;
        scrollArea->setWidget(tabWidget);
        scrollArea->setWidgetResizable(true);
        ui->tabWidget->addTab(scrollArea, fileName);
    }
}

QString MainWindow::makeGroupName(const QString groupName)
{
    return (groupName == "General") ? "" : (groupName + "/");

}

MainWindow::~MainWindow()
{
    for (auto setting : settingsList) delete setting;
    delete ui;
}
