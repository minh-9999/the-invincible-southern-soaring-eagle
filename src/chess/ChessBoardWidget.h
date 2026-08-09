#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include "Piece.h"
#include "../game/move_data.h"
#include "../menus/menu_engine.h"


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

    QMenu* customThemeMenu;
    QAction *actLoadCustomBoard;
    QAction *actLoadCustomPieces;
    QAction *actResetCustomTheme;

    QMenu* m_pieceSetMenu = nullptr;

    bool showArrow;
    bool showCoordinate;

    // Get board categories for piece set selection
    const QMap<QString, QString>& getBoardCategories() const { return boardCategories; }

    // *************************************************************

    explicit ChessBoardWidget(QWidget *parent = nullptr, BoardSize size = BoardSize::Medium);
    ~ChessBoardWidget() override = default;

    void setBoardSize(BoardSize size);
    void resetBoard();
    void loadBoardImage(BoardSize size);
    void updateSizeFromBoard(); // update widget size by sizeLevel

    // Board selection methods
    void loadBoardCategories();
    void createBoardSelectionMenu();
    void loadBoardFromCategory(const QString& category, const QString& imageName);
    void setBoardCategory(const QString& category);

    void PieceSetMenu();

    QString getCurrentBoardCategory() const { return currentBoardCategory; }
    QString getCurrentBoardImage() const { return currentBoardImage; }


    QSize sizeHint() const override;

    QPoint cellToPixel(int row, int col) const;

    QRect getBoardDisplayRect() const;

    void calculateBoardMetrics();
    void analyzeBoardGrid();

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
    QVector<QPoint> validMoves; // Stores valid moves for the currently selected piece

    QString currentSizeFolder = "large"; // or "middle", "small", "mini"

    BoardSize currentSize = BoardSize::Medium;
    BoardSize boardSize;

    static const int boardHeight = 8; // Board height
    static const int boardWidth = 8; // Board width

    PieceSize pieceSize = PieceSize::Medium; // Default piece size is Medium
    int pieceWidth = 30; // Default piece width
    int pieceHeight = 30; // Default piece height

    QString customBoardPath;
    QString customPieceDir;

    QMap<QString, QPixmap> piecePixmaps;

    // Dynamic board metrics calculated from actual board image dimensions
    int calculatedCellWidth = 0;
    int calculatedCellHeight = 0;
    int calculatedLeftMargin = 0;
    int calculatedTopMargin = 0;
    int calculatedPieceSize = 0;

    const int MAX_PIECE_ID = 32;  // If you have 32 pieces

    QVector<Move> moveHistory; // Stores the history of moves
    QPoint currentMoveStart; // Current move start position
    QPoint currentMoveEnd; // Current move end position
    
    // Turn management
    bool isRedTurn = true;  // Red always starts first

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    MenuEngine *menuEngine;
    QString currentEngineBlack;
    QString currentEngineRed;

    void updateEngineSettings();

    int startX = 30;
    int startY = 30;
    int cellWidth = 60;
    int cellHeight = 60;
    int cellSize = 55;

    //
    int selectedRow = -1;
    int selectedCol = -1;
    QVector<QPoint> possibleMoves;

    // Board selection feature
    QMap<QString, QString> boardCategories;  // Category name -> folder path
    QString currentBoardCategory;
    QString currentBoardImage;
    QMenu* boardSelectionMenu = nullptr;

    QMap<QString, QString> pieceCategories;
    QMenu* pieceSelectionMenu = nullptr;

public slots:
    void loadCustomBoard(const QString& path);
    void loadCustomPieces(const QString& dirPath);
    void resetCustomTheme();

    void requestAnalyzeMode();
    void requestChangeMove();
    void requestMoveNow();
    void requestEditPosition();

    void onCopyPosition();

    void onCopyMoveList();

    void onPastePosition();

    // Board and piece selection
    void onBoardChanged(const QString& category, const QString& imagePath);
    void loadPieceSet(const QString& pieceSetPath);
    void calculateBoardMetricsFromImage(const QPixmap& boardImage);
    
    // Move validation
    bool isValidMove(const Piece& piece, int fromRow, int fromCol, int toRow, int toCol) const;
    QVector<QPoint> getValidMovesForPiece(int row, int col) const;

    void onLoadCustomBoard();
    void onLoadCustomPieces();
    void onResetCustomTheme();
    void onLoadPieceSet();
    void onLoadPieceSet(const QString& pieceSetPath);
    void onLoadPieceSetFromCategory();

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
    void invalidMoveAttempted(const QString& pieceName, const QString& moveDescription);

    void boardChanged(const QString& category, const QString& imagePath);
    void pieceSetChanged(const QString& pieceSetPath);

private slots:
    void onAnalysisCompleted();
    void onMoveChanged();
    void onMovePerformed();
    void onPositionEdited();
    void onToggleArrow(bool checked);
    void onToggleCoordinate(bool checked);
void onLoadCustomBoardRequested();
    void onLoadCustomPiecesRequested() ;

    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    void highlightCurrentMove();
    void saveMoveToHistory();
    void clearMoveHistory();
    void validateBoardState();

};


#endif // CHESSBOARDWIDGET_H
