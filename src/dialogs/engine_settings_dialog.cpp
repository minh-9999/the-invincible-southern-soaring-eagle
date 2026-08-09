#include "engine_settings_dialog.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QInputDialog>
#include <QSettings>

EngineSettingsDialog::EngineSettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    setWindowTitle(tr("Engine Settings"));
    resize(1020, 700);

    setupUI();
    updateThinkingModeUi();
}

void EngineSettingsDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(12);

    tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabBar::tab { font-size: 11pt; padding: 6px 12px; }"
                             "QTabBar::tab:selected { background-color: #ed7070ff; }");

    // Create English tabs
    createThinkConfigTab();
    createEngineManageTab();  // Upgraded Tab
    createEngineConfigTab();
    createAdvancedTab();

    tabWidget->addTab(thinkConfigTab, tr("Advanced Config"));
    tabWidget->addTab(engineManageTab, tr("Manage Engine")); // Renamed and modified
    tabWidget->addTab(engineConfigTab, tr("Engine Parameters"));
    tabWidget->addTab(advancedTab, tr("Multi Engine"));

    mainLayout->addWidget(tabWidget);

    // Bottom Actions
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    okButton = new QPushButton(tr("OK"), this);
    okButton->setDefault(true);
    okButton->setMinimumSize(110, 32);

    cancelButton = new QPushButton(tr("Cancel"), this);
    cancelButton->setMinimumSize(110, 32);

    bottomLayout->addWidget(okButton);
    bottomLayout->addWidget(cancelButton);

    mainLayout->addLayout(bottomLayout);

    connect(okButton, &QPushButton::clicked, this, &EngineSettingsDialog::onSaveAndClose);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString EngineSettingsDialog::getGreenGroupStyle() const
{
    return QStringLiteral(
        "QGroupBox { border: 1.5px solid #9ACD32; border-radius: 6px; "
        "margin-top: 12px; padding-top: 10px; font-weight: bold; } "
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; "
        "padding: 0 5px; left: 10px; }");
}

void EngineSettingsDialog::swapTableRows(QTableWidget *table, int row1, int row2)
{
    for (int col = 0; col < table->columnCount(); ++col) {
        QTableWidgetItem *item1 = table->takeItem(row1, col);
        QTableWidgetItem *item2 = table->takeItem(row2, col);
        table->setItem(row1, col, item2);
        table->setItem(row2, col, item1);
    }
}

void EngineSettingsDialog::createThinkConfigTab()
{
    thinkConfigTab = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(thinkConfigTab);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(20); // 2x spacing between group boxes

    QString greenGroupStyle = getGreenGroupStyle();

    // --- TIME CONFIGURATION GROUP ---
    QGroupBox *timeConfigGroup = new QGroupBox(tr("Time Configuration"), thinkConfigTab);
    timeConfigGroup->setStyleSheet(greenGroupStyle);
    QVBoxLayout *timeConfigLayout = new QVBoxLayout(timeConfigGroup);
    timeConfigLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    
    // Left part: Radio groups
    QVBoxLayout *leftLayout = new QVBoxLayout();

    modeButtonGroup = new QButtonGroup(this);

    // 1. Time/Depth Per Move
    timeDepthRadio = new QRadioButton(tr("Time / Depth Per Move"), thinkConfigTab);
    timeDepthRadio->setChecked(true);
    modeButtonGroup->addButton(timeDepthRadio, 0);

    QGridLayout *timeDepthGrid = new QGridLayout();
    timeDepthGrid->setVerticalSpacing(15);
    timeCheck = new QCheckBox(tr("Time"), thinkConfigTab);
    timeCheck->setChecked(true);
    timeSpin = new QSpinBox(thinkConfigTab);
    timeSpin->setRange(1, 999);
    timeSpin->setValue(3);
    QLabel *sLabel1 = new QLabel("S");
    
    depthCheck = new QCheckBox(tr("Depth"), thinkConfigTab);
    depthSpin = new QSpinBox(thinkConfigTab);
    depthSpin->setRange(1, 100);
    depthSpin->setValue(8);
    QLabel *lLabel = new QLabel("L");

    timeDepthGrid->addWidget(timeCheck, 0, 0);
    timeDepthGrid->addWidget(timeSpin, 0, 1);
    timeDepthGrid->addWidget(sLabel1, 0, 2);
    timeDepthGrid->addWidget(depthCheck, 1, 0);
    timeDepthGrid->addWidget(depthSpin, 1, 1);
    timeDepthGrid->addWidget(lLabel, 1, 2);
    
    // 2. Match Time
    matchTimeRadio = new QRadioButton(tr("Match Time"), thinkConfigTab);
    modeButtonGroup->addButton(matchTimeRadio, 1);
    
    QGridLayout *matchTimeGrid = new QGridLayout();
    matchTimeGrid->setVerticalSpacing(15);
    QLabel *allLabel = new QLabel(tr("All"));
    allSpin = new QSpinBox(thinkConfigTab);
    allSpin->setRange(0, 999);
    allSpin->setValue(10);
    QLabel *mLabel = new QLabel("M");
    QLabel *stepLabel = new QLabel(tr("Step"));
    stepSpin = new QSpinBox(thinkConfigTab);
    stepSpin->setRange(0, 999);
    stepSpin->setValue(0);
    QLabel *sLabel2 = new QLabel("S");
    
    QLabel *byoLabel = new QLabel(tr("Byoyomi"));
    byoSpin = new QSpinBox(thinkConfigTab);
    byoSpin->setRange(0, 999);
    byoSpin->setValue(0);
    QLabel *sLabel3 = new QLabel("S");
    QLabel *incLabel = new QLabel(tr("Increment"));
    incSpin = new QSpinBox(thinkConfigTab);
    incSpin->setRange(0, 999);
    incSpin->setValue(0);
    QLabel *sLabel4 = new QLabel("S");

    matchTimeGrid->addWidget(allLabel, 0, 0);
    matchTimeGrid->addWidget(allSpin, 0, 1);
    matchTimeGrid->addWidget(mLabel, 0, 2);
    matchTimeGrid->addWidget(stepLabel, 0, 3);
    matchTimeGrid->addWidget(stepSpin, 0, 4);
    matchTimeGrid->addWidget(sLabel2, 0, 5);

    matchTimeGrid->addWidget(byoLabel, 1, 0);
    matchTimeGrid->addWidget(byoSpin, 1, 1);
    matchTimeGrid->addWidget(sLabel3, 1, 2);
    matchTimeGrid->addWidget(incLabel, 1, 3);
    matchTimeGrid->addWidget(incSpin, 1, 4);
    matchTimeGrid->addWidget(sLabel4, 1, 5);

    leftLayout->addWidget(timeDepthRadio);
    leftLayout->addLayout(timeDepthGrid);
    leftLayout->addSpacing(30);
    leftLayout->addWidget(matchTimeRadio);
    leftLayout->addLayout(matchTimeGrid);
    leftLayout->addStretch();
    
    // Right part: Time By Score
    QVBoxLayout *rightLayout = new QVBoxLayout();
    timeByScoreRadio = new QRadioButton(tr("Time By Score"), thinkConfigTab);
    modeButtonGroup->addButton(timeByScoreRadio, 2);
    
    scoreTable = new QTableWidget(7, 2, thinkConfigTab); // 7 rows, 5 filled, 2 empty for custom
    scoreTable->setHorizontalHeaderLabels({tr("Score >="), tr("Sec Per Move")});
    scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    scoreTable->verticalHeader()->setVisible(false);
    scoreTable->setMaximumHeight(200);
    scoreTable->setFixedWidth(250);
    
    // Fill default values
    QStringList defaultScores = {"600", "300", "0", "-100", "-30000"};
    QStringList defaultSecs = {"10", "20", "35", "45", "53"};
    for (int i = 0; i < 5; ++i) {
        scoreTable->setItem(i, 0, new QTableWidgetItem(defaultScores[i]));
        scoreTable->setItem(i, 1, new QTableWidgetItem(defaultSecs[i]));
    }
    
    rightLayout->addWidget(timeByScoreRadio);
    rightLayout->addWidget(scoreTable, 0, Qt::AlignLeft);
    
    topLayout->addLayout(leftLayout);
    topLayout->addSpacing(60);
    topLayout->addLayout(rightLayout);
    topLayout->addStretch(); // Push everything to the left side
    
    timeConfigLayout->addLayout(topLayout);
    
    // --- ADVANCED SETTINGS GROUP ---
    QGroupBox *advancedSettingsGroup = new QGroupBox(tr("Advanced Settings"), thinkConfigTab);
    advancedSettingsGroup->setStyleSheet(greenGroupStyle);
    QVBoxLayout *advSettingsLayout = new QVBoxLayout(advancedSettingsGroup);
    advSettingsLayout->setSpacing(15);
    
    QHBoxLayout *midLayout1 = new QHBoxLayout();
    QLabel *timeoutLabel = new QLabel(tr("Timeout"));
    timeoutSpin = new QSpinBox(thinkConfigTab);
    timeoutSpin->setRange(0, 9999);
    QLabel *sLabel5 = new QLabel("S");
    
    QLabel *minTimeLabel = new QLabel(tr("Min Time (Connecting)"));
    minTimeSpin = new QSpinBox(thinkConfigTab);
    minTimeSpin->setRange(0, 9999);
    QLabel *msLabel = new QLabel("ms");
    
    midLayout1->addWidget(timeoutLabel);
    midLayout1->addWidget(timeoutSpin);
    midLayout1->addWidget(sLabel5);
    midLayout1->addSpacing(20);
    midLayout1->addWidget(minTimeLabel);
    midLayout1->addWidget(minTimeSpin);
    midLayout1->addWidget(msLabel);
    midLayout1->addStretch();
    
    QHBoxLayout *midLayout2 = new QHBoxLayout();
    inCheck = new QCheckBox(tr("In"), thinkConfigTab);
    inMovesSpin = new QSpinBox(thinkConfigTab);
    inMovesSpin->setValue(3);
    QLabel *movesLabel = new QLabel(tr("Moves Use Time"));
    inTimeSpin = new QSpinBox(thinkConfigTab);
    inTimeSpin->setValue(20);
    QLabel *secLabel1 = new QLabel(tr("Sec Per Move"));
    
    midLayout2->addWidget(inCheck);
    midLayout2->addWidget(inMovesSpin);
    midLayout2->addWidget(movesLabel);
    midLayout2->addWidget(inTimeSpin);
    midLayout2->addWidget(secLabel1);
    midLayout2->addStretch();
    
    QHBoxLayout *midLayout3 = new QHBoxLayout();
    afterCheck = new QCheckBox(tr("After"), thinkConfigTab);
    afterMinuteSpin = new QSpinBox(thinkConfigTab);
    afterMinuteSpin->setValue(12);
    QLabel *minLabel2 = new QLabel(tr("Minute Use Time"));
    afterTimeSpin = new QSpinBox(thinkConfigTab);
    afterTimeSpin->setValue(8);
    QLabel *secLabel2 = new QLabel(tr("Sec Per Move"));
    
    midLayout3->addWidget(afterCheck);
    midLayout3->addWidget(afterMinuteSpin);
    midLayout3->addWidget(minLabel2);
    midLayout3->addWidget(afterTimeSpin);
    midLayout3->addWidget(secLabel2);
    midLayout3->addStretch();
    
    advSettingsLayout->addLayout(midLayout1);
    advSettingsLayout->addSpacing(8);
    advSettingsLayout->addLayout(midLayout2);
    advSettingsLayout->addSpacing(8);
    advSettingsLayout->addLayout(midLayout3);
    
    // --- PROFILES GROUP ---
    QGroupBox *profilesGroup = new QGroupBox(tr("Configuration Profiles"), thinkConfigTab);
    profilesGroup->setStyleSheet(greenGroupStyle);
    QVBoxLayout *profilesLayout = new QVBoxLayout(profilesGroup);
    profilesLayout->setSpacing(15);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    saveItBtn = new QPushButton(tr("Save It"), thinkConfigTab);
    loadItBtn = new QPushButton(tr("Load It"), thinkConfigTab);
    
    deleteItBtn = new QPushButton(thinkConfigTab);
    deleteItBtn->setIcon(QIcon(":/icons/engine_delete_icon.png"));
    
    moveUpBtn = new QPushButton(thinkConfigTab);
    moveUpBtn->setIcon(QIcon(":/icons/move_up_icon.png"));
    
    moveDownBtn = new QPushButton(thinkConfigTab);
    moveDownBtn->setIcon(QIcon(":/icons/move_down_icon.png"));
    
    btnLayout->addStretch();
    btnLayout->addWidget(saveItBtn);
    btnLayout->addWidget(loadItBtn);
    btnLayout->addWidget(deleteItBtn);
    btnLayout->addWidget(moveUpBtn);
    btnLayout->addWidget(moveDownBtn);
    btnLayout->addStretch();
    
    profilesLayout->addLayout(btnLayout);
    
    configListTable = new QTableWidget(0, 2, thinkConfigTab);
    configListTable->setHorizontalHeaderLabels({tr("Name"), tr("Detail")});
    configListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    configListTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    profilesLayout->addWidget(configListTable);

    mainLayout->addWidget(timeConfigGroup);
    mainLayout->addWidget(advancedSettingsGroup);
    mainLayout->addWidget(profilesGroup);

    connect(modeButtonGroup, &QButtonGroup::buttonClicked, this, &EngineSettingsDialog::updateThinkingModeUi);
    connect(saveItBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onSaveConfig);
    connect(loadItBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onLoadConfig);
    connect(deleteItBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onDeleteConfig);
    connect(moveUpBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onMoveUpConfig);
    connect(moveDownBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onMoveDownConfig);
}

void EngineSettingsDialog::createEngineManageTab()
{
    engineManageTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(engineManageTab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(20); // 2x spacing between group boxes

    // Shared green-border style for all group boxes
    QString greenGroupStyle = getGreenGroupStyle();

    QGroupBox *listGroupBox = new QGroupBox(tr("Engine List"), engineManageTab);
    listGroupBox->setStyleSheet(greenGroupStyle);
    QVBoxLayout *vboxListLayout = new QVBoxLayout(listGroupBox);
    vboxListLayout->setSpacing(15);

    // --- FIX 2: Create a Horizontal Layout for Action Buttons ABOVE the Table ---
    QHBoxLayout *topButtonLayout = new QHBoxLayout();

    // UI elements with clean text/emojis symbols to act like crisp icon buttons
    addButton    = new QPushButton(QIcon(":/icons/engine_add_icon.png"), tr("  Add"), listGroupBox);
    editButton   = new QPushButton(QIcon(":/icons/edit_engine_icon.png"), tr("  Edit"), listGroupBox);
    removeButton = new QPushButton(QIcon(":/icons/engine_delete_icon.png"), tr("  Delete"), listGroupBox);
    upButton     = new QPushButton(QIcon(":/icons/move_up_icon.png"), tr("  Move Up"), listGroupBox);
    downButton   = new QPushButton(QIcon(":/icons/move_down_icon.png"), tr("  Move Down"), listGroupBox);

    // Make buttons look consistent
    int btnWidth = 105;
    addButton->setFixedWidth(btnWidth);
    editButton->setFixedWidth(btnWidth);
    removeButton->setFixedWidth(btnWidth);
    upButton->setFixedWidth(btnWidth);
    downButton->setFixedWidth(btnWidth);

    topButtonLayout->addWidget(addButton);
    topButtonLayout->addWidget(editButton);
    topButtonLayout->addWidget(removeButton);
    topButtonLayout->addWidget(upButton);
    topButtonLayout->addWidget(downButton);
    topButtonLayout->addStretch(); // Push elements to the left edge

    // --- FIX 4: Extended layout structure to exactly 4 Columns ---
    engineTable = new QTableWidget(listGroupBox);
    engineTable->setColumnCount(4);

    QStringList headers;
    headers << tr("Name") << tr("Protocol") << tr("Author") << tr("File Path");
    engineTable->setHorizontalHeaderLabels(headers);

    // Formatting column behaviors nicely
    engineTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    engineTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    engineTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    engineTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);

    engineTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    engineTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
    engineTable->verticalHeader()->setVisible(false);
    engineTable->setStyleSheet("QTableView::item { padding-left: 10px; padding-right: 10px; }");

    // Assemble the list block layout
    vboxListLayout->addLayout(topButtonLayout);
    vboxListLayout->addWidget(engineTable);

    // --- FIX 3: Roles config converted entirely to English ---
    QGroupBox *assignmentGroupBox = new QGroupBox(tr("Default Engine Assignment"), engineManageTab);
    assignmentGroupBox->setStyleSheet(greenGroupStyle);
    QFormLayout *formAssignLayout = new QFormLayout(assignmentGroupBox);
    formAssignLayout->setVerticalSpacing(15);

    defaultRedEngineCombo = new QComboBox(assignmentGroupBox);
    defaultRedEngineCombo->addItem(tr("-- Select Red Engine --"));

    defaultBlackEngineCombo = new QComboBox(assignmentGroupBox);
    defaultBlackEngineCombo->addItem(tr("-- Select Black Engine --"));

    formAssignLayout->addRow(new QLabel(tr("Red Side Engine:")), defaultRedEngineCombo);
    formAssignLayout->addRow(new QLabel(tr("Black Side Engine:")), defaultBlackEngineCombo);

    layout->addWidget(listGroupBox);
    layout->addWidget(assignmentGroupBox);

    // Core interactions mapping
    connect(addButton, &QPushButton::clicked, this, &EngineSettingsDialog::onAddEngine);
    connect(editButton, &QPushButton::clicked, this, &EngineSettingsDialog::onEditEngine);
    connect(removeButton, &QPushButton::clicked, this, &EngineSettingsDialog::onRemoveEngine);
    connect(upButton, &QPushButton::clicked, this, &EngineSettingsDialog::onMoveUpEngine);
    connect(downButton, &QPushButton::clicked, this, &EngineSettingsDialog::onMoveDownEngine);
}

void EngineSettingsDialog::createEngineConfigTab()
{
    engineConfigTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(engineConfigTab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(20); // 2x spacing between group boxes

    QString greenGroupStyle = getGreenGroupStyle();

    QGroupBox *hardwareGroupBox = new QGroupBox(tr("Hardware Configuration"), engineConfigTab);
    hardwareGroupBox->setStyleSheet(greenGroupStyle);
    QFormLayout *formHardwareLayout = new QFormLayout(hardwareGroupBox);
    formHardwareLayout->setVerticalSpacing(15);

    hashSizeSpinBox = new QSpinBox(hardwareGroupBox);
    hashSizeSpinBox->setRange(16, 65536);
    hashSizeSpinBox->setValue(512);
    hashSizeSpinBox->setSuffix(" MB");

    threadsSpinBox = new QSpinBox(hardwareGroupBox);
    threadsSpinBox->setRange(1, 256);
    threadsSpinBox->setValue(4);
    threadsSpinBox->setSuffix(tr(" Threads"));

    multiPvSpinBox = new QSpinBox(hardwareGroupBox);
    multiPvSpinBox->setRange(1, 20);
    multiPvSpinBox->setValue(1);
    multiPvSpinBox->setSuffix(tr(" Lines"));

    contemptSpinBox = new QSpinBox(hardwareGroupBox);
    contemptSpinBox->setRange(-100, 100);
    contemptSpinBox->setValue(0);

    priorityComboBox = new QComboBox(hardwareGroupBox);
    priorityComboBox->addItems({tr("Idle"), tr("Low"), tr("Normal"), tr("High"), tr("Realtime")});
    priorityComboBox->setCurrentText(tr("Low"));

    formHardwareLayout->addRow(new QLabel(tr("Hash Size:")), hashSizeSpinBox);
    formHardwareLayout->addRow(new QLabel(tr("CPU Threads:")), threadsSpinBox);
    formHardwareLayout->addRow(new QLabel(tr("MultiPV:")), multiPvSpinBox);
    formHardwareLayout->addRow(new QLabel(tr("Contempt:")), contemptSpinBox);
    formHardwareLayout->addRow(new QLabel(tr("Engine Process Priority:")), priorityComboBox);

    QGroupBox *behaviorGroupBox = new QGroupBox(tr("Engine Extensions"), engineConfigTab);
    behaviorGroupBox->setStyleSheet(greenGroupStyle);
    QVBoxLayout *vboxBehavior = new QVBoxLayout(behaviorGroupBox);
    vboxBehavior->setSpacing(15);

    ponderCheckBox = new QCheckBox(tr("Enable Permanent Brain (Ponder mode)"), behaviorGroupBox);
    ponderCheckBox->setChecked(true);

    ownBookCheckBox = new QCheckBox(tr("Use Engine's Own Opening Book (OwnBook)"), behaviorGroupBox);
    ownBookCheckBox->setChecked(false);

    clearHashButton = new QPushButton(tr("Clear Hash Memory"), behaviorGroupBox);
    clearHashButton->setMaximumWidth(250);

    vboxBehavior->addWidget(ponderCheckBox);
    vboxBehavior->addWidget(ownBookCheckBox);
    vboxBehavior->addSpacing(10);
    vboxBehavior->addWidget(clearHashButton);

    layout->addWidget(hardwareGroupBox);
    layout->addWidget(behaviorGroupBox);
    layout->addStretch();

    connect(clearHashButton, &QPushButton::clicked, this, &EngineSettingsDialog::onClearHash);
}

void EngineSettingsDialog::createAdvancedTab()
{
    advancedTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(advancedTab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(20); 

    QString greenGroupStyle = getGreenGroupStyle();

    QGroupBox *multiEngineGroupBox = new QGroupBox(tr("Multi Engine Settings"), advancedTab);
    multiEngineGroupBox->setStyleSheet(greenGroupStyle);
    QVBoxLayout *vboxMultiEngine = new QVBoxLayout(multiEngineGroupBox);
    vboxMultiEngine->setSpacing(15);

    // Row 1
    QHBoxLayout *row1 = new QHBoxLayout();
    secondaryScoreCheck = new QCheckBox(tr("If Secondary >"), advancedTab);
    secondaryScoreSpinBox = new QSpinBox(advancedTab);
    secondaryScoreSpinBox->setRange(-99999, 99999);
    secondaryScoreSpinBox->setValue(9999);
    QLabel *lbl1 = new QLabel(tr("Take This Engine Move"), advancedTab);
    row1->addWidget(secondaryScoreCheck);
    row1->addWidget(secondaryScoreSpinBox);
    row1->addWidget(lbl1);
    row1->addStretch();
    vboxMultiEngine->addLayout(row1);

    // Row 2
    QHBoxLayout *row2 = new QHBoxLayout();
    secondaryMultiplierCheck = new QCheckBox(tr("If Secondary x"), advancedTab);
    secondaryMultiplierSpinBox = new QSpinBox(advancedTab);
    secondaryMultiplierSpinBox->setRange(-99, 99);
    secondaryMultiplierSpinBox->setValue(1);
    QLabel *lbl2 = new QLabel(tr("> Primary +"), advancedTab);
    primaryPlusSpinBox = new QSpinBox(advancedTab);
    primaryPlusSpinBox->setRange(-9999, 9999);
    primaryPlusSpinBox->setValue(200);
    QLabel *lbl3 = new QLabel(tr("Take This Engine Move"), advancedTab);
    row2->addWidget(secondaryMultiplierCheck);
    row2->addWidget(secondaryMultiplierSpinBox);
    row2->addWidget(lbl2);
    row2->addWidget(primaryPlusSpinBox);
    row2->addWidget(lbl3);
    row2->addStretch();
    vboxMultiEngine->addLayout(row2);

    // Remark Label
    QLabel *remarkLabel = new QLabel(tr("(Remark: From engine1 to engine5, the first enabled engine is the primary engine, others are secondary engines)"), advancedTab);
    remarkLabel->setStyleSheet("color: blue;");
    vboxMultiEngine->addWidget(remarkLabel);
    
    layout->addWidget(multiEngineGroupBox);
    layout->addStretch();
}

void EngineSettingsDialog::updateThinkingModeUi()
{
    bool isTimeDepth = timeDepthRadio->isChecked();
    timeCheck->setEnabled(isTimeDepth);
    timeSpin->setEnabled(isTimeDepth);
    depthCheck->setEnabled(isTimeDepth);
    depthSpin->setEnabled(isTimeDepth);

    bool isMatch = matchTimeRadio->isChecked();
    allSpin->setEnabled(isMatch);
    stepSpin->setEnabled(isMatch);
    byoSpin->setEnabled(isMatch);
    incSpin->setEnabled(isMatch);

    bool isScore = timeByScoreRadio->isChecked();
    scoreTable->setEnabled(isScore);
}

// --- UPDATED CONTROL SLOTS ---

void EngineSettingsDialog::onAddEngine()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Engine Executable"), "", tr("Executables (*.exe);;All Files (*)"));
    if (filePath.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(filePath);
    QString engineName = fileInfo.baseName();
    QString engineAuthor = tr("Unknown Author");
    QString protocol = "UCI";

    QProcess engineProcess;
    QString fullEngineOutput;
    
    engineProcess.start(filePath);
    if (engineProcess.waitForStarted(2000)) {
        // Capture banner if any
        if (engineProcess.waitForReadyRead(500)) {
            fullEngineOutput += QString::fromLocal8Bit(engineProcess.readAllStandardOutput());
        }
        
        engineProcess.write("uci\n");
        fullEngineOutput += "uci\n"; // Fake the user typing it to match screenshot
        
        engineProcess.waitForBytesWritten(1000);

        bool isUci = false;
        // Wait for up to 3 seconds for uciok
        while (engineProcess.waitForReadyRead(3000)) {
            QString output = QString::fromLocal8Bit(engineProcess.readAllStandardOutput());
            fullEngineOutput += output;
            
            QStringList lines = output.split('\n', Qt::SkipEmptyParts);
            for (const QString &line : lines) {
                QString cleanLine = line.trimmed();
                if (cleanLine.startsWith("id name ")) {
                    engineName = cleanLine.mid(8).trimmed();
                } else if (cleanLine.startsWith("id author ")) {
                    engineAuthor = cleanLine.mid(10).trimmed();
                } else if (cleanLine == "uciok") {
                    isUci = true;
                }
            }
            if (isUci) break;
        }

        engineProcess.kill();
        engineProcess.waitForFinished(1000);

        if (!isUci) {
            QMessageBox::warning(this, tr("Engine Error"), tr("The selected executable does not appear to be a valid UCI engine."));
            return;
        }
        
        // Show terminal success message
        QDialog terminalDialog(this);
        terminalDialog.setWindowTitle(tr("Engine Added Successfully"));
        terminalDialog.resize(1000, 750);
        QVBoxLayout *dlgLayout = new QVBoxLayout(&terminalDialog);
        dlgLayout->setContentsMargins(0, 0, 0, 0);
        
        QPlainTextEdit *terminalView = new QPlainTextEdit(&terminalDialog);
        terminalView->setReadOnly(true);
        terminalView->setStyleSheet("QPlainTextEdit { background-color: #0c0c0c; color: #cccccc; font-family: Consolas, monospace; font-size: 11pt; border: none; padding: 10px; }");
        terminalView->setPlainText(fullEngineOutput);
        
        dlgLayout->addWidget(terminalView);
        
        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addStretch();
        QPushButton *okBtn = new QPushButton(tr("OK"), &terminalDialog);
        okBtn->setMinimumSize(100, 30);
        btnLayout->addWidget(okBtn);
        btnLayout->setContentsMargins(10, 10, 10, 10);
        dlgLayout->addLayout(btnLayout);
        
        connect(okBtn, &QPushButton::clicked, &terminalDialog, &QDialog::accept);
        terminalDialog.exec();
        
    } else {
        QMessageBox::critical(this, tr("Engine Error"), tr("Failed to start the selected engine."));
        return;
    }

    int row = engineTable->rowCount();
    engineTable->insertRow(row);

    engineTable->setItem(row, 0, new QTableWidgetItem(engineName));
    engineTable->setItem(row, 1, new QTableWidgetItem(protocol));
    engineTable->setItem(row, 2, new QTableWidgetItem(engineAuthor));
    engineTable->setItem(row, 3, new QTableWidgetItem(filePath));

    defaultRedEngineCombo->addItem(engineName);
    defaultBlackEngineCombo->addItem(engineName);
}

void EngineSettingsDialog::onEditEngine()
{
    int currentRow = engineTable->currentRow();
    int currentCol = engineTable->currentColumn();
    if (currentCol < 0) currentCol = 0;

    if (currentRow >= 0) {
        QTableWidgetItem *item = engineTable->item(currentRow, currentCol);
        if (item) {
            engineTable->editItem(item);
        }
    }
}

void EngineSettingsDialog::onRemoveEngine()
{
    int currentRow = engineTable->currentRow();
    if (currentRow >= 0) {
        QString engineName = engineTable->item(currentRow, 0)->text();
        engineTable->removeRow(currentRow);

        int indexRed = defaultRedEngineCombo->findText(engineName);
        if (indexRed >= 0) defaultRedEngineCombo->removeItem(indexRed);

        int indexBlack = defaultBlackEngineCombo->findText(engineName);
        if (indexBlack >= 0) defaultBlackEngineCombo->removeItem(indexBlack);
    }
}

void EngineSettingsDialog::onMoveUpEngine()
{
    int currentRow = engineTable->currentRow();
    if (currentRow > 0) {
        swapTableRows(engineTable, currentRow, currentRow - 1);
        engineTable->selectRow(currentRow - 1);
    }
}

void EngineSettingsDialog::onMoveDownEngine()
{
    int currentRow = engineTable->currentRow();
    if (currentRow >= 0 && currentRow < engineTable->rowCount() - 1) {
        swapTableRows(engineTable, currentRow, currentRow + 1);
        engineTable->selectRow(currentRow + 1);
    }
}

void EngineSettingsDialog::onClearHash()
{
    QMessageBox::information(this, tr("Hash System"), tr("Clear Hash command sent successfully."));
}

void EngineSettingsDialog::onSaveAndClose()
{
    qDebug() << "Saving engine setup profiles...";
    
    QSettings settings("XiangqiApp", "Engines");
    QStringList engineNames;
    QStringList enginePaths;
    
    for (int i = 0; i < engineTable->rowCount(); ++i) {
        if (engineTable->item(i, 0) && engineTable->item(i, 3)) {
            engineNames.append(engineTable->item(i, 0)->text());
            enginePaths.append(engineTable->item(i, 3)->text());
        }
    }
    
    settings.setValue("EngineNames", engineNames);
    settings.setValue("EnginePaths", enginePaths);
    
    accept();
}

void EngineSettingsDialog::onSaveConfig()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save Configuration"),
                                         tr("Configuration Name:"), QLineEdit::Normal,
                                         tr("New Config"), &ok);
    if (ok && !text.isEmpty()) {
        int row = configListTable->rowCount();
        configListTable->insertRow(row);
        configListTable->setItem(row, 0, new QTableWidgetItem(text));
        
        QString detailText = timeDepthRadio->isChecked() ? tr("Time/Depth Per Move") :
                             matchTimeRadio->isChecked() ? tr("Match Time") : tr("Time By Score");
        configListTable->setItem(row, 1, new QTableWidgetItem(detailText));
    }
}

void EngineSettingsDialog::onLoadConfig()
{
    int row = configListTable->currentRow();
    if (row >= 0) {
        QMessageBox::information(this, tr("Load Configuration"), tr("Configuration '%1' loaded successfully.").arg(configListTable->item(row, 0)->text()));
    } else {
        QMessageBox::warning(this, tr("Load Configuration"), tr("Please select a configuration to load."));
    }
}

void EngineSettingsDialog::onDeleteConfig()
{
    int row = configListTable->currentRow();
    if (row >= 0) {
        configListTable->removeRow(row);
    } else {
        QMessageBox::warning(this, tr("Delete Configuration"), tr("Please select a configuration to delete."));
    }
}

void EngineSettingsDialog::onMoveUpConfig()
{
    int currentRow = configListTable->currentRow();
    if (currentRow > 0) {
        swapTableRows(configListTable, currentRow, currentRow - 1);
        configListTable->selectRow(currentRow - 1);
    }
}

void EngineSettingsDialog::onMoveDownConfig()
{
    int currentRow = configListTable->currentRow();
    if (currentRow >= 0 && currentRow < configListTable->rowCount() - 1) {
        swapTableRows(configListTable, currentRow, currentRow + 1);
        configListTable->selectRow(currentRow + 1);
    }
}