#ifndef BOOK_SETTINGS_DIALOG_H
#define BOOK_SETTINGS_DIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QTableWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>

class BookSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookSettingsDialog(QWidget *parent = nullptr);
    ~BookSettingsDialog() override;

private slots:
    void onAddBook();
    void onEditBook();
    void onRemoveBook();
    void onMoveUpBook();
    void onMoveDownBook();
    void onSaveAndClose();

private:
    void setupUi();
    void loadSettings();
    void saveSettings();

    /// Returns the green border QGroupBox stylesheet matching engine dialog
    QString getGreenGroupStyle() const;

    // LocalBook Tab Components
    QTableWidget *bookTable;
    
    QRadioButton *highestScoreRadio;
    QRadioButton *highestWinRadio;
    QRadioButton *highestCountRadio;
    QRadioButton *fullRandomRadio;
    
    QSpinBox *outOfStepsRedSpin;
    QSpinBox *outOfStepsBlackSpin;
    
    QCheckBox *scoreCheck;
    QSpinBox *scoreSpin;
    
    QCheckBox *winCheck;
    QSpinBox *winSpin;
    
    QCheckBox *countCheck;
    QSpinBox *countSpin;
    
    QCheckBox *scoreOrWinCheck;

    // CloudBook Tab Components
    QCheckBox *getCloudMovesCheck;
    QCheckBox *autoTakeCheck;
    QComboBox *cloudUrlCombo;
    QSpinBox *timeoutSpin;
    QCheckBox *useEndgameOnlyCheck;
    QCheckBox *autoLearnCheck;
    QCheckBox *outOfStepsSameAsLocalCheck;
    QRadioButton *bestRadio;
    QRadioButton *randomRadio;
    QRadioButton *dtcRadio;
    QRadioButton *dtmRadio;
    QRadioButton *localBookFirstRadio;
    QRadioButton *cloudBookFirstRadio;
};

#endif // BOOK_SETTINGS_DIALOG_H
