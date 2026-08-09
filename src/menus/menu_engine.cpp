#include "menu_engine.h"
#include "../chess/ChessBoardWidget.h"  // Include to access ChessBoardWidget
#include "../dialogs/engine_settings_dialog.h"

#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QMenuBar>
#include <QToolBar>
#include <QDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSettings>
#include <QMessageBox>


MenuEngine::MenuEngine(ChessBoardWidget *bWidget)
    : QObject(), boardWidget(bWidget)
{
}

// MenuEngine::MenuEngine(QObject* parent): QObject(parent)
// {
// }

void MenuEngine::engineMenus(QMainWindow* window)
{
    QMenu *engineMenu = new QMenu(tr("Engine"), window);

    QAction *engineSettingAct = engineMenu->addAction(QIcon(":/icons/setting_icon.png"), tr("Engine Setting"));
    engineSettingAct->setShortcut(QKeySequence("Ctrl+K"));

    QAction *engineBlackAct = engineMenu->addAction(QIcon(":/icons/black_icon.png"), tr("Engine Black"));
    engineBlackAct->setShortcut(QKeySequence("Ctrl+B"));

    QAction *engineRedAct = engineMenu->addAction(QIcon(":/icons/red_icon.png"), tr("Engine Red"));
    engineRedAct->setShortcut(QKeySequence("Ctrl+R"));

    QAction *analyzeModeAct = engineMenu->addAction(QIcon(":/icons/analyze_icon.png"), tr("Analyze Mode"));
    analyzeModeAct->setShortcut(QKeySequence("Ctrl+A"));

    // Reuse change-move and move-now actions from boardWidget
    engineMenu->addAction(boardWidget->actChangeMove);
    engineMenu->addAction(boardWidget->actMoveNow);

    window->menuBar()->addMenu(engineMenu);

    // Connect actions to slots
    connect(engineSettingAct, &QAction::triggered, this, &MenuEngine::onEngineSetting);
    connect(engineBlackAct, &QAction::triggered, this, &MenuEngine::onEngineBlack);
    connect(engineRedAct, &QAction::triggered, this, &MenuEngine::onEngineRed);
    connect(analyzeModeAct, &QAction::triggered, this, &MenuEngine::onAnalyzeMode);
    // connect(changeMoveAct, &QAction::triggered, this, &MenuEngine::onChangeMove);
    // connect(moveNowAct, &QAction::triggered, this, &MenuEngine::onMoveNow);

}

void MenuEngine::addEngineActionsToToolBar(QToolBar *toolbar, QWidget* parent)
{
    QAction *engineSettingAct = new QAction(QIcon(":/icons/setting_icon.png"), tr("Engine Setting"), parent);
    toolbar->addAction(engineSettingAct);

    toolbar->addSeparator();

    QAction *engineBlackAct = new QAction(QIcon(":/icons/black_icon.png"), tr("Engine Black"), parent);
    toolbar->addAction(engineBlackAct);

    QAction *engineRedAct = new QAction(QIcon(":/icons/red_icon.png"), tr("Engine Red"), parent);
    toolbar->addAction(engineRedAct);

    toolbar->addSeparator();

    QAction *analyzeModeAct = new QAction(QIcon(":/icons/analyze_icon.png"), tr("Analyze Mode"), parent);
    toolbar->addAction(analyzeModeAct);

    toolbar->addAction(boardWidget->actChangeMove);
    toolbar->addAction(boardWidget->actMoveNow);

    QObject::connect(engineSettingAct, &QAction::triggered, this, &MenuEngine::onEngineSetting);
    QObject::connect(engineBlackAct, &QAction::triggered, this, &MenuEngine::onEngineBlack);
    QObject::connect(engineRedAct, &QAction::triggered, this, &MenuEngine::onEngineRed);
    QObject::connect(analyzeModeAct, &QAction::triggered, this, &MenuEngine::onAnalyzeMode);
}

void MenuEngine::onEngineSetting()
{
    qDebug() << "Engine Settings triggered!";

    // boardWidget->onEngineSetting();

    EngineSettingsDialog dialog(boardWidget);
    dialog.exec(); // Show as modal dialog
}

void MenuEngine::selectEngine(const QString& title)
{
    QDialog dialog;
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(new QLabel(tr("Select an Engine:"), &dialog));
    
    QComboBox *combo = new QComboBox(&dialog);
    QSettings settings("XiangqiApp", "Engines");
    QStringList names = settings.value("EngineNames").toStringList();
    QStringList paths = settings.value("EnginePaths").toStringList();
    
    if (names.isEmpty()) {
        combo->addItem(tr("No engines saved. Go to Engine Settings first."));
        combo->setEnabled(false);
    } else {
        for (int i = 0; i < names.size(); ++i) {
            combo->addItem(names[i], paths.value(i));
        }
    }
    layout->addWidget(combo);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && combo->currentIndex() >= 0 && combo->isEnabled()) {
        QString selectedPath = combo->currentData().toString();
        // TODO: Wire up actual engine start in ChessBoardWidget here
        QMessageBox::information(nullptr, title, tr("Engine selected: %1\nPath: %2").arg(combo->currentText(), selectedPath));
    }
}

void MenuEngine::onEngineBlack()
{
    qDebug() << "Engine Black triggered!";
    selectEngine(tr("Select Engine for Black"));
}

void MenuEngine::onEngineRed()
{
    qDebug() << "Engine Red triggered!";
    selectEngine(tr("Select Engine for Red"));
}

void MenuEngine::onAnalyzeMode()
{
    qDebug() << "Analyze Mode triggered!";
    // Trigger the Analyze Mode
    boardWidget->requestAnalyzeMode();
}
