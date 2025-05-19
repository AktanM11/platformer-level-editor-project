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
    level = new QTableWidget(10, 100);
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

    QPushButton *EraseButton = new QPushButton(QIcon("data/sprites/eraser.png"), "");
    EraseButton->setFixedSize(50, 50);
    connect(EraseButton, &QPushButton::clicked, this, [this](){
        eraseMode = !eraseMode;
    });
    QPushButton *ClearLevelButton = new QPushButton(QIcon("data/sprites/trashcan.png"), "");
    ClearLevelButton->setFixedSize(50, 50);
    connect(ClearLevelButton, &QPushButton::clicked, this, &MainWindow::clearLevel);
    buttonLayout->addWidget(ClearLevelButton);

    buttonLayout->addWidget(EraseButton);
    mainLayout->addLayout(buttonLayout);
    centralWidget->show();

    QToolBar *toolBar = addToolBar("File Operations");

    QAction *exportAction = new QAction("Export Level", this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportToFile);
    toolBar->addAction(exportAction);
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
    if (eraseMode) {
        item->setIcon(QIcon());
        item->setData(Qt::UserRole, ' ');
    } else {
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
            case TileType::Coin: data = '*'; break;
            case TileType::Enemy: data = '&'; break;
            case TileType::Exit: data = 'E'; break;
            case TileType::Player: data = '@'; break;
            case TileType::Spikes: data = '^'; break;
            case TileType::Wall: data = '#'; break;
            case TileType::DarkWall: data = '='; break;
        }
        item->setData(Qt::UserRole, data);
    }
}

QPushButton* MainWindow::createButton(const QIcon &icon, TileType tileType, QHBoxLayout* layout)
{
    QPushButton *button = new QPushButton();

    button->setFixedSize(50, 50);
    button->setIcon(icon);
    connect(button, &QPushButton::clicked, this, [this,  tileType](){
        selectTile(static_cast<char>(tileType));
        eraseMode = false;});
    layout->addWidget(button);
    return button;
}

void MainWindow::clearLevel()
{
    for (int row = 0; row < level->rowCount(); ++row) {
        for (int col = 0; col < level->columnCount(); ++col) {
            QTableWidgetItem* item = level->item(row, col);
            if (item != nullptr) {
                item->setIcon(QIcon());
                item->setData(Qt::UserRole, ' ');
            }
        }
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

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export Error", "Failed to open the file for writing.");
        return;
    }

    QTextStream out(&file);

    int rows = level->rowCount();
    int cols = level->columnCount();
    std::vector<char> data(rows * cols, '-');

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            QTableWidgetItem* item = level->item(row, col);
            if (item && item->data(Qt::UserRole).isValid()) {
                char tile = item->data(Qt::UserRole).toChar().toLatin1();
                data[row * cols + col] = tile;
            }
        }
    }

    QString output;
    encrypt(rows, cols, data.data(), output);

    if (output.isEmpty()) {
        QMessageBox::warning(this, "Export Error", "Encryption failed. File not saved.");
    } else {
        out << output;
    }

    file.close();
    QMessageBox::information(this, "Export Successful", "Level exported successfully!");
}

