#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets>

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void selectTile(char tile);
    void onTileClicked(int row, int col);
    void undoTilePlacement();

    void clearLevel();
    void resizeLevel();

    void exportToFile();

    struct TileAction
    {
        // TODO
    };

    enum class TileType
    {
        Coin     = '*',
        Enemy    = '&',
        Exit     = 'E',
        Player   = '@',
        Spikes   = '^',
        Wall     = '#',
        DarkWall = '=',
        Air      = ' '
    };
    QPushButton* createButton(const QIcon &icon, TileType tileType, QHBoxLayout* layout);
    TileType selectedTile;
    QStack<TileAction> undoStack;

    /*
    QTableWidget *level;
            OR
    QGraphicsGridLayout *level;
    */

    QTableWidget* level = nullptr;
};

#endif // MAIN_WINDOW_H
