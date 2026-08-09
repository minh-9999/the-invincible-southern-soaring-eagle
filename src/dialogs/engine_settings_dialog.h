#ifndef ENGINE_SETTINGS_DIALOG_H
#define ENGINE_SETTINGS_DIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>
#include <QButtonGroup>

/**
 * @brief Dialog for configuring chess engine settings.
 *
 * Provides four tabs:
 *  - Advanced Config: Time control, depth, match time, time-by-score, profiles.
 *  - Manage Engine: Add/edit/remove engines, assign default sides.
 *  - Engine Parameters: UCI options (hash, threads, MultiPV, contempt, priority).
 *  - Multi Engine: Secondary engine decision rules.
 */
class EngineSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EngineSettingsDialog(QWidget *parent = nullptr);
    ~EngineSettingsDialog() override = default;

private:
    // --- Main layout ---
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;

    // Global control buttons
    QPushButton *okButton;
    QPushButton *cancelButton;

    // --- TAB 1: ADVANCED CONFIG ---
    QWidget *thinkConfigTab;
    QButtonGroup *modeButtonGroup;
    QRadioButton *timeDepthRadio;
    QRadioButton *matchTimeRadio;
    QRadioButton *timeByScoreRadio;

    QCheckBox *timeCheck;
    QSpinBox *timeSpin;
    QCheckBox *depthCheck;
    QSpinBox *depthSpin;

    QSpinBox *allSpin;
    QSpinBox *stepSpin;
    QSpinBox *byoSpin;
    QSpinBox *incSpin;

    QTableWidget *scoreTable;

    QSpinBox *timeoutSpin;
    QSpinBox *minTimeSpin;
    QCheckBox *inCheck;
    QSpinBox *inMovesSpin;
    QSpinBox *inTimeSpin;
    QCheckBox *afterCheck;
    QSpinBox *afterMinuteSpin;
    QSpinBox *afterTimeSpin;

    QPushButton *saveItBtn;
    QPushButton *loadItBtn;
    QPushButton *deleteItBtn;
    QPushButton *moveUpBtn;
    QPushButton *moveDownBtn;

    QTableWidget *configListTable;

    // --- TAB 2: MANAGE ENGINE ---
    QWidget *engineManageTab;
    QTableWidget *engineTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *upButton;
    QPushButton *downButton;

    QComboBox *defaultRedEngineCombo;
    QComboBox *defaultBlackEngineCombo;

    // --- TAB 3: ENGINE PARAMETERS (UCI Options) ---
    QWidget *engineConfigTab;
    QSpinBox *hashSizeSpinBox;
    QSpinBox *threadsSpinBox;
    QSpinBox *multiPvSpinBox;
    QSpinBox *contemptSpinBox;
    QComboBox *priorityComboBox;
    QCheckBox *ponderCheckBox;
    QCheckBox *ownBookCheckBox;
    QPushButton *clearHashButton;

    // --- TAB 4: MULTI ENGINE ---
    QWidget *advancedTab;
    QCheckBox *secondaryScoreCheck;
    QSpinBox *secondaryScoreSpinBox;
    QCheckBox *secondaryMultiplierCheck;
    QSpinBox *secondaryMultiplierSpinBox;
    QSpinBox *primaryPlusSpinBox;

    // Interface build methods
    void setupUI();
    void createThinkConfigTab();
    void createEngineManageTab();
    void createEngineConfigTab();
    void createAdvancedTab();

    void updateThinkingModeUi();

    // Helper: returns the shared QGroupBox green-border stylesheet.
    QString getGreenGroupStyle() const;

    // Helper: swaps two adjacent rows in a QTableWidget.
    void swapTableRows(QTableWidget *table, int row1, int row2);

private slots:
    void onAddEngine();
    void onEditEngine();
    void onRemoveEngine();
    void onMoveUpEngine();
    void onMoveDownEngine();
    void onClearHash();
    void onSaveAndClose();
    void onSaveConfig();
    void onLoadConfig();
    void onDeleteConfig();
    void onMoveUpConfig();
    void onMoveDownConfig();
};

#endif // ENGINE_SETTINGS_DIALOG_H