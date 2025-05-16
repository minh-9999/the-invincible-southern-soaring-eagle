#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include "Piece.h"
#include "move_data.h"
#include "menu_engine.h"


class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:
    enum class BoardSize { Mini, Small, Medium, Large };

    enum class PieceSize { Small, Medium, Large };

    QAction* copyPositionAct;
    QAction* copyMoveListAct;
    QAction* pastePositionAct;

    QAction *actCopyPosition;
    QAction *actCopyMoveList;
    QAction *actPastePosition;
    QAction *actAnalyzeMode;
    QAction *actChangeMove;
    QAction *actMoveNow;
    QAction *actEditPosition;

    QAction *smallPieceAct;
    QAction *mediumPieceAct;
    QAction *largePieceAct;
    QAction *toggleArrowAct;
    QAction *toggleCoordinateAct;
    QMenu *pieceSizeMenu;

    bool showArrow;
    bool showCoordinate;

    // *************************************************************

    void loadBoardImage(BoardSize size);

    // Constructor with default parameters for boardSize
    explicit ChessBoardWidget(QWidget *parent = nullptr, BoardSize size = BoardSize::Medium);

    void setBoardSize(BoardSize size);

    QSize sizeHint() const override;

    void resetBoard();

    QPoint cellToPixel(int row, int col) const;

    QRect getBoardDisplayRect() const;

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    void showContextMenu(const QPoint &pos);
    void loadFromFile(const QString& fileName);
    void saveAsXQF(const QString &fileName);
    void saveAsPGN(const QString &fileName);

    QString getXQFFormat();

    bool loadPGN(const QString& filePath);
    bool loadXQF(const QString& filePath);
    bool loadCHE(const QString& filePath);
    bool loadMXQ(const QString& filePath);

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    QString getPositionData(); // Returns a string describing the position of the flag
    void setPositionData(const QString& data); // Pastes the data onto the board
    QString getMoveList(); // Returns the list of moves
    void setMoveList(const QString& moveList); // Pastes the list of moves onto the board
    bool isValidPositionData(const QString& data); // Checks if the position data is valid
    bool isValidMoveList(const QString& data); // Checks if the list of moves is valid
    void reverseBoard(); // Rotates the board

    // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    void setupInitialPosition();
    void stepForward();
    bool isValidPiece(const QString& piece);
    void setPieceSize(PieceSize size);
    // void updatePieceSize();

    QString getPieceSymbol(int row, int col);
    void swapPieces(int row1, int col1, int row2, int col2);

    // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    void applyMove(const Move& move);
    const Piece* getPieceAt(int x, int y) const;
    // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    void onEngineSetting();
    void onEngineBlack();
    void onEngineRed();

    // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

protected:
    QPixmap removeBorder(const QPixmap &pixmap);
    QPixmap autoRemoveBorder(const QPixmap& pixmap, int tolerance);
    QPixmap addGlowEffect(const QPixmap &src, QColor glowColor);
    QPixmap addDropShadowEffect(const QPixmap &src);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void loadPieceImages();
    QRect getCellRect(int row, int col) const;
    QPixmap getPiecePixmap(const Piece &piece) const;

private:

    QVector<QVector<Piece>> board;
    QPoint selectedCell = {-1, -1}; // currently selected cell
    QPixmap boardImage; // chessboard background image

    QString baseImagePath;

    int currentMoveIndex = 0; // Current move index
    QVector<QString> moves;   //list of moves, examples: ["炮二平五", "马８进７", ...]

    QList<QString> gameMoves;

    QString currentSizeFolder = "large"; // or "middle", "small", "mini"

    void updateSizeFromBoard(); // update widget size by sizeLevel
    BoardSize currentSize = BoardSize::Medium;
    BoardSize boardSize;

    static const int boardHeight = 8; // Board height
    static const int boardWidth = 8; // Board width

    PieceSize pieceSize = PieceSize::Medium; // Default piece size is Medium
    int pieceWidth = 30; // Default piece width
    int pieceHeight = 30; // Default piece height

    QMap<QString, QPixmap> piecePixmaps;

    const int MAX_PIECE_ID = 32;  // If you have 32 pieces



    QVector<Move> moveHistory; // Stores the history of moves
    QPoint currentMoveStart; // Current move start position
    QPoint currentMoveEnd; // Current move end position

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    MenuEngine *menuEngine;
    QString currentEngineBlack;
    QString currentEngineRed;

    void updateEngineSettings();

public slots:
    void requestAnalyzeMode();
    void requestChangeMove();
    void requestMoveNow();
    void requestEditPosition();

    void onCopyPosition();

    void onCopyMoveList();

    void onPastePosition();

signals:
    void copyPositionRequested();
    void copyMoveListRequested();
    void pastePositionRequested();

    void analysisCompleted();
    void moveChanged();
    void movePerformed();
    void positionEdited();
    void toggleArrow(bool checked);
    void toggleCoordinate(bool checked);

private slots:
    void onAnalysisCompleted();
    void onMoveChanged();
    void onMovePerformed();
    void onPositionEdited();
    void onToggleArrow(bool checked);
    void onToggleCoordinate(bool checked);

    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    void highlightCurrentMove();
    void saveMoveToHistory();
    void clearMoveHistory();
    void validateBoardState();



};


#endif // CHESSBOARDWIDGET_H
