#include "book_settings_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QToolBar>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QComboBox>
#include <QLineEdit>

BookSettingsDialog::BookSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    setWindowTitle(tr("OpenBook Settings"));
    resize(700, 600);

    setupUi();
    loadSettings();
}

QString BookSettingsDialog::getGreenGroupStyle() const
{
    return QStringLiteral(
        "QGroupBox { border: 1.5px solid #9ACD32; border-radius: 6px; "
        "margin-top: 12px; padding-top: 10px; font-weight: bold; } "
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; "
        "padding: 0 5px; left: 10px; }");
}

BookSettingsDialog::~BookSettingsDialog()
{
}

void BookSettingsDialog::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(12);

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabBar::tab { font-size: 11pt; padding: 6px 12px; }"
                             "QTabBar::tab:selected { background-color: #ed7070ff; }");

    QString greenStyle = getGreenGroupStyle();

    // ----------------------------------------------------
    // Tab 1: LocalBook
    // ----------------------------------------------------
    QWidget *localBookTab = new QWidget(tabWidget);
    QVBoxLayout *localBookLayout = new QVBoxLayout(localBookTab);
    localBookLayout->setContentsMargins(14, 14, 14, 14);
    localBookLayout->setSpacing(20);

    // GroupBox: LocalBook Manage
    QGroupBox *manageGroupBox = new QGroupBox(tr("LocalBook Manage"), localBookTab);
    manageGroupBox->setStyleSheet(greenStyle);
    QVBoxLayout *manageLayout = new QVBoxLayout(manageGroupBox);

    // Toolbar inside manage
    QToolBar *manageToolbar = new QToolBar(manageGroupBox);
    manageToolbar->setIconSize(QSize(24, 24));
    
    QAction *addAct = manageToolbar->addAction(QIcon(":/icons/add_icon.png"), tr("Add"));
    QAction *editAct = manageToolbar->addAction(QIcon(":/icons/edit_icon.png"), tr("Edit"));
    QAction *removeAct = manageToolbar->addAction(QIcon(":/icons/delete_icon.png"), tr("Remove"));
    manageToolbar->addSeparator();
    QAction *moveUpAct = manageToolbar->addAction(QIcon(":/icons/move_up_icon.png"), tr("Move Up"));
    QAction *moveDownAct = manageToolbar->addAction(QIcon(":/icons/move_down_icon.png"), tr("Move Down"));

    connect(addAct, &QAction::triggered, this, &BookSettingsDialog::onAddBook);
    connect(editAct, &QAction::triggered, this, &BookSettingsDialog::onEditBook);
    connect(removeAct, &QAction::triggered, this, &BookSettingsDialog::onRemoveBook);
    connect(moveUpAct, &QAction::triggered, this, &BookSettingsDialog::onMoveUpBook);
    connect(moveDownAct, &QAction::triggered, this, &BookSettingsDialog::onMoveDownBook);

    manageLayout->addWidget(manageToolbar);

    // Table
    bookTable = new QTableWidget(0, 3, manageGroupBox);
    bookTable->setHorizontalHeaderLabels({tr("Name"), tr("Type"), tr("FilePath")});
    bookTable->horizontalHeader()->setStretchLastSection(true);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    manageLayout->addWidget(bookTable);

    localBookLayout->addWidget(manageGroupBox);

    // GroupBox: LocalBook Config
    QGroupBox *configGroupBox = new QGroupBox(tr("LocalBook Config"), localBookTab);
    configGroupBox->setStyleSheet(greenStyle);
    QHBoxLayout *configLayout = new QHBoxLayout(configGroupBox);

    // GroupBox: Selection Strategy
    QGroupBox *strategyGroupBox = new QGroupBox(tr("Selection Strategy"), configGroupBox);
    QVBoxLayout *strategyLayout = new QVBoxLayout(strategyGroupBox);
    
    highestScoreRadio = new QRadioButton(tr("Highest Score"), strategyGroupBox);
    highestWinRadio = new QRadioButton(tr("Highest Win%"), strategyGroupBox);
    highestCountRadio = new QRadioButton(tr("Highest Count"), strategyGroupBox);
    fullRandomRadio = new QRadioButton(tr("Full Random"), strategyGroupBox);
    highestScoreRadio->setChecked(true); // default
    
    strategyLayout->addWidget(highestScoreRadio);
    strategyLayout->addWidget(highestWinRadio);
    strategyLayout->addWidget(highestCountRadio);
    strategyLayout->addWidget(fullRandomRadio);
    
    strategyLayout->addSpacing(10);
    strategyLayout->addWidget(new QLabel(tr("Out Of Steps (0 Means Never)"), strategyGroupBox));
    
    QHBoxLayout *outOfStepsLayout = new QHBoxLayout();
    outOfStepsLayout->addWidget(new QLabel(tr("Red"), strategyGroupBox));
    outOfStepsRedSpin = new QSpinBox(strategyGroupBox);
    outOfStepsRedSpin->setMaximum(999);
    outOfStepsLayout->addWidget(outOfStepsRedSpin);
    
    outOfStepsLayout->addWidget(new QLabel(tr("Black"), strategyGroupBox));
    outOfStepsBlackSpin = new QSpinBox(strategyGroupBox);
    outOfStepsBlackSpin->setMaximum(999);
    outOfStepsLayout->addWidget(outOfStepsBlackSpin);
    outOfStepsLayout->addStretch();
    
    strategyLayout->addLayout(outOfStepsLayout);
    configLayout->addWidget(strategyGroupBox);

    // GroupBox: Limitation
    QGroupBox *limitationGroupBox = new QGroupBox(tr("Limitation"), configGroupBox);
    QVBoxLayout *limitationLayout = new QVBoxLayout(limitationGroupBox);
    
    QHBoxLayout *scoreLayout = new QHBoxLayout();
    scoreCheck = new QCheckBox(tr("Score >="), limitationGroupBox);
    scoreSpin = new QSpinBox(limitationGroupBox);
    scoreSpin->setRange(-9999, 9999);
    scoreLayout->addWidget(scoreCheck);
    scoreLayout->addWidget(scoreSpin);
    scoreLayout->addStretch();
    limitationLayout->addLayout(scoreLayout);
    
    QHBoxLayout *winLayout = new QHBoxLayout();
    winCheck = new QCheckBox(tr("Win% >="), limitationGroupBox);
    winSpin = new QSpinBox(limitationGroupBox);
    winSpin->setRange(0, 100);
    winLayout->addWidget(winCheck);
    winLayout->addWidget(winSpin);
    winLayout->addWidget(new QLabel("%", limitationGroupBox));
    winLayout->addStretch();
    limitationLayout->addLayout(winLayout);
    
    QHBoxLayout *countLayout = new QHBoxLayout();
    countCheck = new QCheckBox(tr("Count >="), limitationGroupBox);
    countSpin = new QSpinBox(limitationGroupBox);
    countSpin->setMaximum(999999);
    countLayout->addWidget(countCheck);
    countLayout->addWidget(countSpin);
    countLayout->addStretch();
    limitationLayout->addLayout(countLayout);
    
    limitationLayout->addSpacing(10);
    scoreOrWinCheck = new QCheckBox(tr("Score Or Win% Just Need One"), limitationGroupBox);
    limitationLayout->addWidget(scoreOrWinCheck);
    
    configLayout->addWidget(limitationGroupBox);

    localBookLayout->addWidget(configGroupBox);
    tabWidget->addTab(localBookTab, tr("LocalBook"));

    // ----------------------------------------------------
    // Tab 2: CloudBook
    // ----------------------------------------------------
    QWidget *cloudBookTab = new QWidget(tabWidget);
    QVBoxLayout *cloudLayout = new QVBoxLayout(cloudBookTab);
    cloudLayout->setContentsMargins(14, 14, 14, 14);
    cloudLayout->setSpacing(20);

    // Top checkboxes row
    QHBoxLayout *cloudTopRow = new QHBoxLayout();
    getCloudMovesCheck = new QCheckBox(tr("Get Cloud Moves"), cloudBookTab);
    getCloudMovesCheck->setChecked(true);
    autoTakeCheck = new QCheckBox(tr("Auto Take"), cloudBookTab);
    cloudTopRow->addWidget(getCloudMovesCheck);
    cloudTopRow->addSpacing(30);
    cloudTopRow->addWidget(autoTakeCheck);
    cloudTopRow->addStretch();
    cloudLayout->addLayout(cloudTopRow);

    // GroupBox: CloudBook Config
    QGroupBox *cloudConfigGroup = new QGroupBox(tr("CloudBook Config"), cloudBookTab);
    cloudConfigGroup->setStyleSheet(greenStyle);
    QVBoxLayout *cloudConfigLayout = new QVBoxLayout(cloudConfigGroup);
    cloudConfigLayout->setSpacing(15);

    // Cloud URL
    QHBoxLayout *urlLayout = new QHBoxLayout();
    urlLayout->addWidget(new QLabel(tr("Cloud URL"), cloudConfigGroup));
    cloudUrlCombo = new QComboBox(cloudConfigGroup);
    cloudUrlCombo->setEditable(true);
    cloudUrlCombo->addItem("http://api.chessdb.cn:81/chessdb.php");
    cloudUrlCombo->setMinimumWidth(350);
    urlLayout->addWidget(cloudUrlCombo);
    urlLayout->addStretch();
    cloudConfigLayout->addLayout(urlLayout);

    // Timeout
    QHBoxLayout *timeoutLayout = new QHBoxLayout();
    timeoutLayout->addWidget(new QLabel(tr("Timeout"), cloudConfigGroup));
    timeoutSpin = new QSpinBox(cloudConfigGroup);
    timeoutSpin->setRange(100, 60000);
    timeoutSpin->setValue(2000);
    timeoutSpin->setSuffix(" ms");
    timeoutLayout->addWidget(timeoutSpin);
    timeoutLayout->addStretch();
    cloudConfigLayout->addLayout(timeoutLayout);

    // Checkbox row: Use EndGame Only / Auto Learn / Out Of Steps Same As LocalB
    QHBoxLayout *cloudCheckRow = new QHBoxLayout();
    useEndgameOnlyCheck = new QCheckBox(tr("Use EndGame Only"), cloudConfigGroup);
    autoLearnCheck = new QCheckBox(tr("Auto Learn"), cloudConfigGroup);
    autoLearnCheck->setChecked(true);
    outOfStepsSameAsLocalCheck = new QCheckBox(tr("Out Of Steps Same As LocalB"), cloudConfigGroup);
    cloudCheckRow->addWidget(useEndgameOnlyCheck);
    cloudCheckRow->addSpacing(15);
    cloudCheckRow->addWidget(autoLearnCheck);
    cloudCheckRow->addSpacing(15);
    cloudCheckRow->addWidget(outOfStepsSameAsLocalCheck);
    cloudCheckRow->addStretch();
    cloudConfigLayout->addLayout(cloudCheckRow);

    // Selection Strategy + EndGame Type side by side
    QHBoxLayout *stratEndgameRow = new QHBoxLayout();

    QGroupBox *cloudStratGroup = new QGroupBox(tr("Selection Strategy"), cloudConfigGroup);
    cloudStratGroup->setStyleSheet(greenStyle);
    QHBoxLayout *cloudStratLayout = new QHBoxLayout(cloudStratGroup);
    bestRadio = new QRadioButton(tr("Best"), cloudStratGroup);
    bestRadio->setChecked(true);
    randomRadio = new QRadioButton(tr("Random"), cloudStratGroup);
    cloudStratLayout->addWidget(bestRadio);
    cloudStratLayout->addSpacing(20);
    cloudStratLayout->addWidget(randomRadio);
    cloudStratLayout->addStretch();
    stratEndgameRow->addWidget(cloudStratGroup);

    QGroupBox *endgameTypeGroup = new QGroupBox(tr("EndGame Type"), cloudConfigGroup);
    endgameTypeGroup->setStyleSheet(greenStyle);
    QHBoxLayout *endgameTypeLayout = new QHBoxLayout(endgameTypeGroup);
    dtcRadio = new QRadioButton(tr("DTC"), endgameTypeGroup);
    dtcRadio->setChecked(true);
    dtmRadio = new QRadioButton(tr("DTM"), endgameTypeGroup);
    endgameTypeLayout->addWidget(dtcRadio);
    endgameTypeLayout->addSpacing(20);
    endgameTypeLayout->addWidget(dtmRadio);
    endgameTypeLayout->addStretch();
    stratEndgameRow->addWidget(endgameTypeGroup);

    cloudConfigLayout->addLayout(stratEndgameRow);

    // Order group
    QGroupBox *orderGroup = new QGroupBox(tr("Order"), cloudConfigGroup);
    orderGroup->setStyleSheet(greenStyle);
    QHBoxLayout *orderLayout = new QHBoxLayout(orderGroup);
    localBookFirstRadio = new QRadioButton(tr("LocalBook First"), orderGroup);
    localBookFirstRadio->setChecked(true);
    cloudBookFirstRadio = new QRadioButton(tr("CloudBook First"), orderGroup);
    orderLayout->addWidget(localBookFirstRadio);
    orderLayout->addSpacing(40);
    orderLayout->addWidget(cloudBookFirstRadio);
    orderLayout->addStretch();
    cloudConfigLayout->addWidget(orderGroup);

    cloudLayout->addWidget(cloudConfigGroup);

    // Credits label
    QLabel *creditsLabel = new QLabel(tr("Thank to Nooby who provides the CloudBook (www.chessdb.cn)"), cloudBookTab);
    creditsLabel->setStyleSheet("color: #999999; font-style: italic;");
    creditsLabel->setAlignment(Qt::AlignCenter);
    cloudLayout->addStretch();
    cloudLayout->addWidget(creditsLabel);

    tabWidget->addTab(cloudBookTab, tr("CloudBook"));

    mainLayout->addWidget(tabWidget);

    // Bottom Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton *okBtn = new QPushButton(tr("OK"), this);
    okBtn->setMinimumWidth(100);
    okBtn->setStyleSheet("QPushButton { font-weight: bold; border: 2px solid #2196F3; padding: 5px; }");
    QPushButton *cancelBtn = new QPushButton(tr("Cancel"), this);
    cancelBtn->setMinimumWidth(100);
    cancelBtn->setStyleSheet("QPushButton { padding: 5px; }");
    
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);

    connect(okBtn, &QPushButton::clicked, this, &BookSettingsDialog::onSaveAndClose);
    connect(cancelBtn, &QPushButton::clicked, this, &BookSettingsDialog::reject);
}

void BookSettingsDialog::loadSettings()
{
    QSettings settings("XiangqiApp", "BookSettings");
    
    // Load Strategy
    QString strategy = settings.value("SelectionStrategy", "HighestScore").toString();
    if (strategy == "HighestWin") highestWinRadio->setChecked(true);
    else if (strategy == "HighestCount") highestCountRadio->setChecked(true);
    else if (strategy == "FullRandom") fullRandomRadio->setChecked(true);
    else highestScoreRadio->setChecked(true);
    
    outOfStepsRedSpin->setValue(settings.value("OutOfStepsRed", 0).toInt());
    outOfStepsBlackSpin->setValue(settings.value("OutOfStepsBlack", 0).toInt());
    
    // Load Limitation
    scoreCheck->setChecked(settings.value("ScoreLimitEnabled", true).toBool());
    scoreSpin->setValue(settings.value("ScoreLimit", 0).toInt());
    
    winCheck->setChecked(settings.value("WinLimitEnabled", false).toBool());
    winSpin->setValue(settings.value("WinLimit", 10).toInt());
    
    countCheck->setChecked(settings.value("CountLimitEnabled", false).toBool());
    countSpin->setValue(settings.value("CountLimit", 0).toInt());
    
    scoreOrWinCheck->setChecked(settings.value("ScoreOrWinNeedOne", false).toBool());
    
    // Load Books
    int bookCount = settings.beginReadArray("LocalBooks");
    for (int i = 0; i < bookCount; ++i) {
        settings.setArrayIndex(i);
        QString name = settings.value("Name").toString();
        bool enabled = settings.value("Enabled", true).toBool();
        QString type = settings.value("Type", "Normal").toString();
        QString path = settings.value("FilePath").toString();
        
        int row = bookTable->rowCount();
        bookTable->insertRow(row);
        
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        nameItem->setFlags(nameItem->flags() | Qt::ItemIsUserCheckable);
        nameItem->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
        bookTable->setItem(row, 0, nameItem);
        
        bookTable->setItem(row, 1, new QTableWidgetItem(type));
        bookTable->setItem(row, 2, new QTableWidgetItem(path));
    }
    settings.endArray();

    // Load CloudBook settings
    getCloudMovesCheck->setChecked(settings.value("CloudGetMoves", true).toBool());
    autoTakeCheck->setChecked(settings.value("CloudAutoTake", false).toBool());
    cloudUrlCombo->setCurrentText(settings.value("CloudUrl", "http://api.chessdb.cn:81/chessdb.php").toString());
    timeoutSpin->setValue(settings.value("CloudTimeout", 2000).toInt());
    useEndgameOnlyCheck->setChecked(settings.value("CloudUseEndgameOnly", false).toBool());
    autoLearnCheck->setChecked(settings.value("CloudAutoLearn", true).toBool());
    outOfStepsSameAsLocalCheck->setChecked(settings.value("CloudOutOfStepsSameAsLocal", false).toBool());
    bestRadio->setChecked(settings.value("CloudStrategyBest", true).toBool());
    randomRadio->setChecked(!settings.value("CloudStrategyBest", true).toBool());
    dtcRadio->setChecked(settings.value("CloudEndgameDTC", true).toBool());
    dtmRadio->setChecked(!settings.value("CloudEndgameDTC", true).toBool());
    localBookFirstRadio->setChecked(settings.value("CloudLocalBookFirst", true).toBool());
    cloudBookFirstRadio->setChecked(!settings.value("CloudLocalBookFirst", true).toBool());
}

void BookSettingsDialog::saveSettings()
{
    QSettings settings("XiangqiApp", "BookSettings");
    
    // Save Strategy
    QString strategy = "HighestScore";
    if (highestWinRadio->isChecked()) strategy = "HighestWin";
    else if (highestCountRadio->isChecked()) strategy = "HighestCount";
    else if (fullRandomRadio->isChecked()) strategy = "FullRandom";
    settings.setValue("SelectionStrategy", strategy);
    
    settings.setValue("OutOfStepsRed", outOfStepsRedSpin->value());
    settings.setValue("OutOfStepsBlack", outOfStepsBlackSpin->value());
    
    // Save Limitation
    settings.setValue("ScoreLimitEnabled", scoreCheck->isChecked());
    settings.setValue("ScoreLimit", scoreSpin->value());
    
    settings.setValue("WinLimitEnabled", winCheck->isChecked());
    settings.setValue("WinLimit", winSpin->value());
    
    settings.setValue("CountLimitEnabled", countCheck->isChecked());
    settings.setValue("CountLimit", countSpin->value());
    
    settings.setValue("ScoreOrWinNeedOne", scoreOrWinCheck->isChecked());
    
    // Save Books
    settings.beginWriteArray("LocalBooks");
    for (int i = 0; i < bookTable->rowCount(); ++i) {
        settings.setArrayIndex(i);
        QTableWidgetItem *nameItem = bookTable->item(i, 0);
        settings.setValue("Name", nameItem->text());
        settings.setValue("Enabled", nameItem->checkState() == Qt::Checked);
        settings.setValue("Type", bookTable->item(i, 1)->text());
        settings.setValue("FilePath", bookTable->item(i, 2)->text());
    }
    settings.endArray();

    // Save CloudBook settings
    settings.setValue("CloudGetMoves", getCloudMovesCheck->isChecked());
    settings.setValue("CloudAutoTake", autoTakeCheck->isChecked());
    settings.setValue("CloudUrl", cloudUrlCombo->currentText());
    settings.setValue("CloudTimeout", timeoutSpin->value());
    settings.setValue("CloudUseEndgameOnly", useEndgameOnlyCheck->isChecked());
    settings.setValue("CloudAutoLearn", autoLearnCheck->isChecked());
    settings.setValue("CloudOutOfStepsSameAsLocal", outOfStepsSameAsLocalCheck->isChecked());
    settings.setValue("CloudStrategyBest", bestRadio->isChecked());
    settings.setValue("CloudEndgameDTC", dtcRadio->isChecked());
    settings.setValue("CloudLocalBookFirst", localBookFirstRadio->isChecked());
}

void BookSettingsDialog::onAddBook()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select OpenBook File"), "", tr("Book Files (*.obk *.bin);;All Files (*)"));
    if (filePath.isEmpty()) return;
    
    QFileInfo fileInfo(filePath);
    int row = bookTable->rowCount();
    bookTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(fileInfo.fileName());
    nameItem->setFlags(nameItem->flags() | Qt::ItemIsUserCheckable);
    nameItem->setCheckState(Qt::Checked);
    
    bookTable->setItem(row, 0, nameItem);
    bookTable->setItem(row, 1, new QTableWidgetItem("Normal"));
    bookTable->setItem(row, 2, new QTableWidgetItem(filePath));
}

void BookSettingsDialog::onEditBook()
{
    int row = bookTable->currentRow();
    if (row >= 0) {
        QTableWidgetItem *item = bookTable->item(row, 2); // Edit file path
        if (item) {
            QString filePath = QFileDialog::getOpenFileName(this, tr("Edit OpenBook File"), item->text(), tr("Book Files (*.obk *.bin);;All Files (*)"));
            if (!filePath.isEmpty()) {
                item->setText(filePath);
                QFileInfo fileInfo(filePath);
                bookTable->item(row, 0)->setText(fileInfo.fileName());
            }
        }
    }
}

void BookSettingsDialog::onRemoveBook()
{
    int row = bookTable->currentRow();
    if (row >= 0) {
        bookTable->removeRow(row);
    }
}

void BookSettingsDialog::onMoveUpBook()
{
    int row = bookTable->currentRow();
    if (row > 0) {
        // Swap rows logic
        for (int col = 0; col < bookTable->columnCount(); ++col) {
            QTableWidgetItem *item1 = bookTable->takeItem(row, col);
            QTableWidgetItem *item2 = bookTable->takeItem(row - 1, col);
            bookTable->setItem(row - 1, col, item1);
            bookTable->setItem(row, col, item2);
        }
        bookTable->selectRow(row - 1);
    }
}

void BookSettingsDialog::onMoveDownBook()
{
    int row = bookTable->currentRow();
    if (row >= 0 && row < bookTable->rowCount() - 1) {
        for (int col = 0; col < bookTable->columnCount(); ++col) {
            QTableWidgetItem *item1 = bookTable->takeItem(row, col);
            QTableWidgetItem *item2 = bookTable->takeItem(row + 1, col);
            bookTable->setItem(row + 1, col, item1);
            bookTable->setItem(row, col, item2);
        }
        bookTable->selectRow(row + 1);
    }
}

void BookSettingsDialog::onSaveAndClose()
{
    saveSettings();
    accept();
}
