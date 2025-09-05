#ifndef ENGINE_SETTINGS_DIALOG_H
#define ENGINE_SETTINGS_DIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include "ui_EngineSettingsDialog.h"

class EngineSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    EngineSettingsDialog(QWidget *parent = nullptr);

private:
    QTableWidget *engineTable;
    QPushButton *addButton, *editButton, *removeButton, *upButton, *downButton;
    QTabWidget *tabWidget;
    QPushButton *okButton, *cancelButton;

    Ui::EngineSettingsDialog ui;

    void setupUI();
    void setupThinkConfigTab(QWidget *tab);

private slots:
    // void onSaveConfig();
    // void onLoadConfig();
};

#endif // ENGINE_SETTINGS_DIALOG_H
