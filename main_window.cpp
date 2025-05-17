#include "main_window.h"
#include "utilities.h"
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), selectedTile(TileType::Air)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Platformer Level Editor");
    setFocusPolicy(Qt::StrongFocus);
    centralWidget->setFocusPolicy(Qt::StrongFocus);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    level = new QTableWidget(10,10);
    level->resize(30, 20);
    level->verticalHeader()->setDefaultSectionSize(24);
    level->horizontalHeader()->setDefaultSectionSize(24);
    level->verticalHeader()->setMinimumSectionSize(24);
    level->horizontalHeader()->setMinimumSectionSize(24);
    level->verticalHeader()->hide();
    level->horizontalHeader()->hide();
    level->setEditTriggers(QAbstractItemView::NoEditTriggers);

    level->setSelectionMode(QAbstractItemView::NoSelection);
    level->setSelectionBehavior(QAbstractItemView::SelectItems);
    connect(level, &QTableWidget::cellClicked, this, &MainWindow::onTileClicked);

    mainLayout->addWidget(level);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *WallButton = createButton(QIcon("data/sprites/wall.png"), TileType::Wall, buttonLayout);
    QPushButton *ExitButton = createButton(QIcon("data/sprites/exit.png"), TileType::Exit, buttonLayout);
    QPushButton *CoinButton = createButton(QIcon("data/sprites/coin.png"), TileType::Coin, buttonLayout);
    QPushButton *EnemyButton = createButton(QIcon("data/sprites/enemy.png"), TileType::Enemy, buttonLayout);
    QPushButton *PlayerButton = createButton(QIcon("data/sprites/player.png"), TileType::Player, buttonLayout);
    QPushButton *SpikeButton = createButton(QIcon("data/sprites/spikes.png"), TileType::Spikes, buttonLayout);
    QPushButton *DarkWallButton = createButton(QIcon("data/sprites/wall_dark.png"), TileType::DarkWall, buttonLayout);

    mainLayout->addLayout(buttonLayout);
    centralWidget->show();
}

MainWindow::~MainWindow() { }

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // TODO

    QMainWindow::keyPressEvent(event);
}

void MainWindow::selectTile(char tile)
{
    switch(tile) {
        case '*': selectedTile = TileType::Coin;     break;
        case '&': selectedTile = TileType::Enemy;    break;
        case 'E': selectedTile = TileType::Exit;     break;
        case '@': selectedTile = TileType::Player;   break;
        case '^': selectedTile = TileType::Spikes;   break;
        case '#': selectedTile = TileType::Wall;     break;
        case '=': selectedTile = TileType::DarkWall; break;
        case ' ': selectedTile = TileType::Air;      break;
    }
}

void MainWindow::onTileClicked(int row, int col)
{
    QTableWidgetItem* item = level->item(row, col);
    if (item == nullptr) {
        item = new QTableWidgetItem();
        level->setItem(row, col, item);
    }
    QIcon icon;
    switch (selectedTile) {
        case TileType::Coin: icon = QIcon("data/sprites/coin.png"); break;
        case TileType::Enemy: icon = QIcon("data/sprites/enemy.png"); break;
        case TileType::Exit: icon = QIcon("data/sprites/exit.png"); break;
        case TileType::Player: icon = QIcon("data/sprites/player.png"); break;
        case TileType::Spikes: icon = QIcon("data/sprites/spikes.png"); break;
        case TileType::Wall: icon = QIcon("data/sprites/wall.png"); break;
        case TileType::DarkWall: icon = QIcon("data/sprites/wall_dark.png"); break;
    }

    item->setIcon(icon);
    char data = ' ';
    switch (selectedTile) {
        case TileType::Coin:  data = '*'; break;
        case TileType::Enemy: data = '&'; break;
        case TileType::Exit: data = 'E'; break;
        case TileType::Player: data = '@'; break;
        case TileType::Spikes: data = '^'; break;
        case TileType::Wall: data = '#'; break;
        case TileType::DarkWall: data = '='; break;
    }
    item->setData(Qt::UserRole, data);
    // item = currentLevel()->item(row, col);
    // item->setData(Qt::UserRole, tile);
}

void MainWindow::undoTilePlacement()
{
    if (undoStack.isEmpty()) return;
    TileAction action = undoStack.pop();

    // TODO
}

QPushButton* MainWindow::createButton(const QIcon &icon, TileType tileType, QHBoxLayout* layout)
{
    QPushButton *button = new QPushButton();

    button->setFixedSize(32, 32);
    button->setIcon(icon);
    connect(button, &QPushButton::clicked, this, [this,  tileType](){selectTile(static_cast<char>(tileType));});
    layout->addWidget(button);
    return button;
}

void MainWindow::clearLevel()
{
    // TODO
}

void MainWindow::resizeLevel()
{
    QDialog resizeDialog(this);
    // TODO
    if (resizeDialog.exec() == QDialog::Accepted) {
        // TODO
    }
}

void MainWindow::exportToFile()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export Level",
        "",
        "RLL Files (*.rll);;All Files (*)"
    );

    QFile file(filePath);
    QTextStream out(&file);

    // TODO

    /* Snippets:
     * std::vector<char> data(rows * cols, '-');
     * char tile = item->data(Qt::UserRole).toChar().toLatin1();
     * encrypt(rows, cols, data.data(), output);
     * out << output;
     */

    file.close();
}
