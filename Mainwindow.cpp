#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "KeyStrings.h"
#include "KeyInputBox.h"
#include <QtWidgets>

const QString backupSuffix = ".backup";
const int statusMessageDelay = 2000;

MainWindow::MainWindow() :
    QMainWindow(),
    ui(new Ui::MainWindow),
    setting(nullptr),
    settingWidget(nullptr),
    fileSystemModel(new QFileSystemModel),
    generalGroupName(tr("General"))
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

    settingWidget = new QLabel(tr("\n      Choose a file to edit."), ui->scrollArea);

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
            [=]() {
        loadSetting(getSelectedFilePath());
    });

    connect(ui->action_Backup, &QAction::triggered,
            [=]() {
        const QString fileName = getSelectedFilePath();
        const QString backupName = fileName + backupSuffix;
        if (QFile::exists(backupName)) {
            QFile::remove(backupName);
        }
        QFile::copy(fileName, backupName);
        ui->action_Restore->setEnabled(true);
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
            ui->statusBar->showMessage(tr("File is copied to clipboard."), statusMessageDelay);
        }
    });

    connect(ui->actionClose,    &QAction::triggered, &QApplication::quit);
    connect(ui->actionAbout,    &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::showAboutQt);
}

QString MainWindow::getSelectedFilePath() const {
    return fileSystemModel->filePath(ui->treeView->selectionModel()->selectedIndexes().at(0));
}

void MainWindow::loadSetting(const QString fileName) {
    QVBoxLayout * const layout = new QVBoxLayout;

    delete setting;
    setting = new QSettings(fileName, QSettings::IniFormat);

    const QStringList groups = QStringList(generalGroupName) << setting->childGroups();
    for (const QString groupName : groups) {
        if (groupName != generalGroupName) setting->beginGroup(groupName);
        if (groups.size() != 1) layout->addWidget(new QLabel("<h1>" + groupName + "</h1>"));

        for (const QString key : setting->childKeys()) {
            QHBoxLayout * const lineLayout = new QHBoxLayout;

            lineLayout->addWidget(new QLabel(key));

            const QString type = key.left(3);
            const QString keyName = makeGroupName(groupName) + key;

            if (type == "num" || type == "per") {
                QSpinBox * const spinBox = new QSpinBox;
                if (type == "per") {
                    spinBox->setSuffix("%");
                    spinBox->setMaximum(100);
                } else {
                    spinBox->setMaximum(std::numeric_limits<int>::max());
                }
                spinBox->setValue(setting->value(key, 0).toInt());
                connect(spinBox, &QSpinBox::editingFinished,
                        [=]() {
                    setting->setValue(keyName, spinBox->value());
                });
                lineLayout->addWidget(spinBox);
            } else if (type == "is_") {
                QCheckBox * const checkBox = new QCheckBox;
                checkBox->setChecked(setting->value(key, false).toBool());
                connect(checkBox, &QCheckBox::clicked,
                        [=](const bool checked) {
                    setting->setValue(keyName, checked);
                });
                lineLayout->addWidget(checkBox);
            } else if (type == "key") {
                const QString oldValue = setting->value(key).toString();
                QLabel * const keyLabel = new QLabel(oldValue == "" ? NoKey : oldValue);

                QPushButton * const setButton = new QPushButton(tr("Set"));
                connect(setButton, &QPushButton::clicked,
                        [=]() {
                    const int newKey = KeyInputBox::GetKey(this);
                    if (newKey) {
                        const QString newValue = GetKeyString(newKey);
                        setting->setValue(keyName, newValue);
                        keyLabel->setText(newValue);
                    }
                });

                QPushButton * const removeButton = new QPushButton(tr("Remove"));
                connect(removeButton, &QPushButton::clicked,
                        [=]() {
                    setting->setValue(keyName, NoKey);
                    keyLabel->setText(NoKey);
                });

                lineLayout->addWidget(keyLabel);
                lineLayout->addWidget(setButton);
                lineLayout->addWidget(removeButton);
            } else { // text field
                QLineEdit * const lineEdit = new QLineEdit(setting->value(key).toString());
                lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
                connect(lineEdit, &QLineEdit::editingFinished,
                        [=]() {
                    setting->setValue(keyName, lineEdit->text());
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

    ui->statusBar->showMessage(tr("File \"%1\" loaded.").arg(fileName), statusMessageDelay);
}

void MainWindow::showAbout() {
    QFile readmeFile(":/README.md");
    readmeFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QMessageBox::about(this, tr("Optiom, version %1").arg(VER), readmeFile.readAll());
}

void MainWindow::showAboutQt() {
    QMessageBox::aboutQt(this);
}

QString MainWindow::makeGroupName(QString groupName) const {
    return (groupName == generalGroupName) ? "" : (groupName.append("/"));
}

MainWindow::~MainWindow() {
    delete setting;
    delete settingWidget;
    delete ui;
}
