#include "engine_settings_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <QHeaderView>


EngineSettingsDialog::EngineSettingsDialog(QWidget *parent) : QDialog(parent)
{
    // Set flags to make the window have full Minimize, Maximize, Close buttons
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

    // Window title
    setWindowTitle("Engine Settings");

    // Initial size
    resize(1000, 600);

    // Define your own internal interface
    setupUI();

    // Populate table data (if there is an engine list, etc.)
    // populateTable();
}

void EngineSettingsDialog::setupUI()
{
    tabWidget = new QTabWidget(this);

    // Create child tabs, here are placeholder widgets
    QWidget *thinkConfigTab = new QWidget();
    QWidget *engineManageTab = new QWidget();
    QWidget *engineConfigTab = new QWidget();
    QWidget *multiEngineTab = new QWidget();

    // Add tabs to QTabWidget
    tabWidget->addTab(thinkConfigTab, "Think Config");
    tabWidget->addTab(engineManageTab, "Engine Manage");
    tabWidget->addTab(engineConfigTab, "Engine Config");
    tabWidget->addTab(multiEngineTab, "Multi Engine");

    // Create OK and Cancel buttons
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    QFont font = okButton->font();
    font.setPointSize(14);
    okButton->setFont(font);
    cancelButton->setFont(font);

    // Layout for 2 buttons at the bottom, horizontally on the right
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addSpacing(100);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    // Main layout, vertically
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Connect the signal for the Cancel button to close the dialog
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    // Connect the signal for the OK button, you customize the handling
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
}

void EngineSettingsDialog::setupThinkConfigTab(QWidget *tab) {
    // === Group 1: Time / Depth / Match ===
    QGroupBox *groupBox = new QGroupBox(tab);
    groupBox->setTitle("");

    // Radio buttons
    QRadioButton *timeDepthRadio = new QRadioButton("Time/Depth Per Move");
    QRadioButton *scoreRadio = new QRadioButton("Time By Score");
    QRadioButton *matchTimeRadio = new QRadioButton("Match Time");

    timeDepthRadio->setChecked(true);  // mặc định

    // Time/Depth input
    QCheckBox *timeCheck = new QCheckBox("Time");
    QSpinBox *timeSpin = new QSpinBox();
    timeSpin->setRange(0, 999);
    QLabel *sLabel1 = new QLabel("S");

    QCheckBox *depthCheck = new QCheckBox("Depth");
    QSpinBox *depthSpin = new QSpinBox();
    depthSpin->setRange(0, 999);
    QLabel *lLabel = new QLabel("L");

    // Time by score table
    QTableWidget *scoreTable = new QTableWidget(5, 2);
    scoreTable->setHorizontalHeaderLabels(QStringList() << "Score >=" << "Sec Per Move");
    QStringList scores = {"600", "300", "0", "-100", "-30000"};
    QStringList times = {"10", "20", "35", "45", "53"};
    for (int i = 0; i < 5; ++i) {
        scoreTable->setItem(i, 0, new QTableWidgetItem(scores[i]));
        scoreTable->setItem(i, 1, new QTableWidgetItem(times[i]));
    }

    // Match Time section
    QLabel *allLabel = new QLabel("All");
    QSpinBox *allSpin = new QSpinBox(); allSpin->setRange(0, 999);
    QLabel *mLabel = new QLabel("M");
    QLabel *stepLabel = new QLabel("Step");
    QSpinBox *stepSpin = new QSpinBox(); stepSpin->setRange(0, 999);
    QLabel *sLabel2 = new QLabel("S");

    QLabel *byoLabel = new QLabel("Byo");
    QSpinBox *byoSpin = new QSpinBox();
    QLabel *incLabel = new QLabel("Inc");
    QSpinBox *incSpin = new QSpinBox();
    QLabel *sLabel3 = new QLabel("S");

    // Timeout & Min time
    QLabel *timeoutLabel = new QLabel("Timeout");
    QSpinBox *timeoutSpin = new QSpinBox();
    QLabel *timeoutS = new QLabel("S");

    QLabel *minTimeLabel = new QLabel("Min Time(Connecting)");
    QSpinBox *minTimeSpin = new QSpinBox();
    QLabel *minTimeMs = new QLabel("ms");

    // "In X Moves Use Time Y Sec Per Move"
    QCheckBox *inMovesCheck = new QCheckBox("In");
    QSpinBox *inMovesSpin = new QSpinBox();
    QLabel *movesUseTimeLabel = new QLabel("Moves Use Time");
    QSpinBox *secPerMove1 = new QSpinBox();
    QLabel *secPerLabel1 = new QLabel("Sec Per Move");

    // "After X Minute Use Time Y Sec Per Move"
    QCheckBox *afterMinuteCheck = new QCheckBox("After");
    QSpinBox *afterMinuteSpin = new QSpinBox();
    QLabel *minuteUseTimeLabel = new QLabel("Minute Use Time");
    QSpinBox *secPerMove2 = new QSpinBox();
    QLabel *secPerLabel2 = new QLabel("Sec Per Move");

    // Layouting
    QGridLayout *grid = new QGridLayout(groupBox);

    // Line 0: radio buttons
    grid->addWidget(timeDepthRadio, 0, 0, 1, 3);
    grid->addWidget(scoreRadio, 0, 3, 1, 3);

    // Line 1: time/depth + table
    grid->addWidget(timeCheck, 1, 0);
    grid->addWidget(timeSpin, 1, 1);
    grid->addWidget(sLabel1, 1, 2);

    grid->addWidget(depthCheck, 1, 3);
    grid->addWidget(depthSpin, 1, 4);
    grid->addWidget(lLabel, 1, 5);

    grid->addWidget(scoreTable, 2, 3, 5, 3);

    // Line 2: Match time
    grid->addWidget(matchTimeRadio, 3, 0, 1, 2);
    grid->addWidget(allLabel, 4, 0);
    grid->addWidget(allSpin, 4, 1);
    grid->addWidget(mLabel, 4, 2);
    grid->addWidget(stepLabel, 4, 3);
    grid->addWidget(stepSpin, 4, 4);
    grid->addWidget(sLabel2, 4, 5);

    grid->addWidget(byoLabel, 5, 0);
    grid->addWidget(byoSpin, 5, 1);
    grid->addWidget(incLabel, 5, 2);
    grid->addWidget(incSpin, 5, 3);
    grid->addWidget(sLabel3, 5, 4);

    // Line 3: Timeout
    grid->addWidget(timeoutLabel, 6, 0);
    grid->addWidget(timeoutSpin, 6, 1);
    grid->addWidget(timeoutS, 6, 2);
    grid->addWidget(minTimeLabel, 6, 3);
    grid->addWidget(minTimeSpin, 6, 4);
    grid->addWidget(minTimeMs, 6, 5);

    // Line 4: In Moves Use Time
    grid->addWidget(inMovesCheck, 7, 0);
    grid->addWidget(inMovesSpin, 7, 1);
    grid->addWidget(movesUseTimeLabel, 7, 2);
    grid->addWidget(secPerMove1, 7, 3);
    grid->addWidget(secPerLabel1, 7, 4);

    // Line 5: After Minute Use Time
    grid->addWidget(afterMinuteCheck, 8, 0);
    grid->addWidget(afterMinuteSpin, 8, 1);
    grid->addWidget(minuteUseTimeLabel, 8, 2);
    grid->addWidget(secPerMove2, 8, 3);
    grid->addWidget(secPerLabel2, 8, 4);

    // Cuối cùng gán layout cho tab
    tab->setLayout(grid);

    // === Group 2: Save / Load Config ===
    QGroupBox *bottomGroup = new QGroupBox(tab);
    bottomGroup->setTitle("");

    QPushButton *saveBtn = new QPushButton("Save It");
    QPushButton *updateBtn = new QPushButton("Update It");
    QPushButton *loadBtn = new QPushButton("Load It");
    QPushButton *delBtn = new QPushButton("Del It");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(updateBtn);
    buttonLayout->addWidget(loadBtn);
    buttonLayout->addWidget(delBtn);
    bottomGroup->setLayout(buttonLayout);

    // === Table: Config Name & Detail ===
    QTableWidget *configTable = new QTableWidget();
    configTable->setColumnCount(2);
    configTable->setHorizontalHeaderLabels(QStringList() << "Name" << "Detail");
    configTable->horizontalHeader()->setStretchLastSection(true);
    configTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    configTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Dummy rows (bạn có thể thay bằng dữ liệu thực sau này)
    configTable->setRowCount(2);
    configTable->setItem(0, 0, new QTableWidgetItem("Fast Mode"));
    configTable->setItem(0, 1, new QTableWidgetItem("10s per move"));

    configTable->setItem(1, 0, new QTableWidgetItem("Blitz Mode"));
    configTable->setItem(1, 1, new QTableWidgetItem("3m total + 2s increment"));

    // === Gộp tất cả thành layout chính cho tab ===
    QVBoxLayout *mainLayout = new QVBoxLayout(tab);
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(bottomGroup);
    mainLayout->addWidget(configTable);
    tab->setLayout(mainLayout);

    // connect(saveBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onSaveConfig);
    // connect(loadBtn, &QPushButton::clicked, this, &EngineSettingsDialog::onLoadConfig);
}


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

// void EngineSettingsDialog::onAddEngine()
// {
//     int row = engineTable->rowCount();
//     engineTable->insertRow(row);
//     engineTable->setItem(row, 0, new QTableWidgetItem("New Engine"));
//     engineTable->setItem(row, 1, new QTableWidgetItem("/path/to/engine"));

//     QPushButton* editBtn = new QPushButton("Edit");
//     connect(editBtn, &QPushButton::clicked, this, [=]() {
//         qDebug() << "Edit new engine at row:" << row;
//     });

//     engineTable->setCellWidget(row, 2, editBtn);
// }

// void EngineSettingsDialog::onRemoveEngine()
// {
//     QList<QTableWidgetSelectionRange> selected = engineTable->selectedRanges();

//     if (!selected.isEmpty())
//     {
//         int row = selected.first().topRow();
//         engineTable->removeRow(row);
//     }
// }

