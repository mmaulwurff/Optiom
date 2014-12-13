#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "KeyStrings.h"
#include "KeyInputBox.h"
#include <QtWidgets>

const QString backupSuffix = ".backup";

MainWindow::MainWindow(QWidget * const parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    setting(nullptr),
    settingWidget(nullptr),
    fileSystemModel(new QFileSystemModel)
{
    ui->setupUi(this);
    setWindowTitle(QDir::currentPath());

    ui->actionReload->setShortcut(QKeySequence::Refresh);

    fileSystemModel->setNameFilters(QStringList("*.ini"));
    fileSystemModel->setNameFilterDisables(false);
    fileSystemModel->setRootPath(QDir::currentPath());
    ui->treeView->setModel(fileSystemModel);
    ui->treeView->setRootIndex(fileSystemModel->index(QDir::currentPath()));
    ui->treeView->hideColumn(2);
    ui->treeView->setColumnWidth(0, 150);

    connect(ui->treeView, &QTreeView::activated, ui->treeView, &QTreeView::clicked);
    connect(ui->treeView, &QTreeView::clicked,
            [=](const QModelIndex & index) {
        if (not QFileInfo(fileSystemModel->filePath(index)).isDir()) {
            loadSetting(fileSystemModel->filePath(index));
        }
    });
    connect(ui->treeView, &QTreeView::clicked,
            [=](const QModelIndex & index) {
        const QString fileName = fileSystemModel->filePath(index);
        const bool disable = QFileInfo(fileName).isDir();
        ui->action_Backup->setDisabled(disable);
        ui->action_Restore->setDisabled(disable || not QFile::exists(fileName + backupSuffix));
        ui->actionCopy_file_to_clipboard->setDisabled(disable);
    });

    connect(ui->actionReload, &QAction::triggered,
            [=]() { loadSetting(getSelectedFilePath()); });

    connect(ui->action_Backup, &QAction::triggered,
            [=]() {
        const QString fileName = getSelectedFilePath();
        const QString backupName = fileName + backupSuffix;
        if (QFile::exists(backupName)) {
            QFile::remove(backupName);
        }
        QFile::copy(fileName, backupName);
        ui->statusBar->showMessage(tr("Backup \"%1\" created.").arg(backupName));
    });
    connect(ui->action_Restore, &QAction::triggered,
            [=]() {
        const QString fileName = getSelectedFilePath();
        QFile::remove(fileName);
        QFile::copy(fileName + ".backup", fileName);
        loadSetting(fileName);
        ui->statusBar->showMessage(tr("Backup \"%1\" loaded.").arg(fileName + backupSuffix));
    });

    connect(ui->actionCopy_file_to_clipboard, &QAction::triggered,
            [=]() {
        QFile settingsFile(getSelectedFilePath());
        if (settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString contents(settingsFile.readAll());
            QApplication::clipboard()->setText(contents);
        }
    });

    connect(ui->actionClose,    &QAction::triggered, &QApplication::quit);
    connect(ui->actionAbout ,   &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::showAboutQt);
}

QString MainWindow::getSelectedFilePath() const {
    return fileSystemModel->filePath(ui->treeView->selectionModel()->selectedIndexes().at(0));
}

void MainWindow::loadSetting(const QString fileName) {
    ui->statusBar->showMessage("");

    QVBoxLayout * const layout = new QVBoxLayout;

    delete setting;
    setting = new QSettings(fileName, QSettings::IniFormat);

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
            } else if (type == "key") {
                QLabel * const keyLabel = new QLabel(setting->value(key, NoKey).toString());
                QPushButton * const setButton = new QPushButton("Set");

                QPushButton * const removeButton = new QPushButton("Remove");
                connect(removeButton, &QPushButton::clicked, [=]() {
                    setting->setValue(makeGroupName(groupName) + key, "");
                    keyLabel->setText(NoKey);
                });

                connect(setButton, &QPushButton::clicked, [=]() {
                    const int newKey = KeyInputBox::GetKey(this);
                    if (newKey) {
                        const QString newValue = GetKeyString(newKey);
                        setting->setValue(makeGroupName(groupName) + key, newValue);
                        keyLabel->setText(newValue);
                    }
                });

                lineLayout->addWidget(keyLabel);
                lineLayout->addWidget(setButton);
                lineLayout->addWidget(removeButton);
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

    layout->addStretch();
    delete settingWidget;
    settingWidget = new QWidget();
    settingWidget->setLayout(layout);
    ui->scrollArea->setWidget(settingWidget);
    ui->scrollArea->setWidgetResizable(true);
}

void MainWindow::showAbout()
{
    QFile readmeFile(":/README.md");
    readmeFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QMessageBox::about(this, "About Optiom", readmeFile.readAll());
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this, "Optiom");
}

QString MainWindow::makeGroupName(const QString groupName)
{
    return (groupName == "General") ? "" : (groupName + "/");
}

MainWindow::~MainWindow()
{
    delete setting;
    delete settingWidget;
    delete ui;
}
