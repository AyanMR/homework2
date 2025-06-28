#include "gamewindow.h"
#include "ui_GameWindow.h"
#include "random.h"
std::set< int > vec1, player1, player2, player3;
std::set< int > curPlayerPoker;
int             curPlayer         = 0;
int             player1PokerCount = 17;
int             player2PokerCount = 17;
int             player3PokerCount = 17;


GameWindow::GameWindow(QWidget *parent) : QWidget(parent), ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    checkboxForPoker = new QGridLayout(ui->widget_2);
    ui->widget_2->setLayout(checkboxForPoker);
    std::vector< int > poker = genRanNum();
    for (int i = 0 ; i < 3 ; ++i)
        vec1.insert(poker[i]);
    for (int i = 3 ; i < 20 ; ++i)
        player1.insert(poker[i]);
    for (int i = 20 ; i < 37 ; ++i)
        player2.insert(poker[i]);
    for (int i = 37 ; i < 54 ; ++i)
        player3.insert(poker[i]);
    playerList = {1, 2, 3};
    lordRound  = 0;
    nextPlayer();
    connect(ui->pushButton_3, &QPushButton::clicked, this, &GameWindow::getLord);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &GameWindow::getLord);

    connect(ui->pushButton, &QPushButton::clicked, this, [this] {
        std::set< int > &currentPoker = curPlayer == 1 ?
                                            player1 :
                                            curPlayer == 2 ?
                                                player2 :
                                                player3;

        qDebug() << curPlayer << " sum:" << currentPoker.size();

        int col = 0;
        int row = 0;
        for (auto poker : currentPoker)
        {
            QString imagePath = QString(":/poker/%1.png").arg(poker);
            QLabel *label     = new QLabel(ui->widget_2);
            label->setText(QString::number(poker));

            QPixmap pixMap(imagePath);
            if (pixMap.isNull())
                qDebug() << "not find image";
            else
            {
                pixMap = pixMap.scaled(128, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                label->setPixmap(pixMap);
            }
            label->setProperty("value", poker);
            label->setFrameShape(QFrame::Box);
            label->setLineWidth(2);
            label->setStyleSheet("border: 2px solid transparent;");
            label->installEventFilter(this);
            checkboxForPoker->addWidget(label, row, col ++);
            if (col > 7)
            {
                col = 0;
                row ++;
            }
        }
    });
    connect(ui->pushButton_4, &QPushButton::clicked, this, &GameWindow::clearPoker);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &GameWindow::clearPoker);
}

void GameWindow::clearPoker()
{
    QLayoutItem *ch;
    while ((ch = checkboxForPoker->takeAt(0)) != nullptr)
    {
        delete ch->widget();
        delete ch;
    }
};

bool GameWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QLabel *cardLabel = qobject_cast< QLabel * >(watched);
        if (cardLabel)
        {
            int cardValue = cardLabel->property("value").toInt();
            if (selectedCards.count(cardValue))
            {
                selectedCards.erase(cardValue);
                cardLabel->setStyleSheet("border: 2px solid transparent;");
            }
            else
            {
                selectedCards.insert(cardValue);
                cardLabel->setStyleSheet("border: 2px solid red;");
            }

            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}


void GameWindow::nextPlayer()
{
    auto it = playerList.find(curPlayer);
    if (it == playerList.end())
        curPlayer = *playerList.begin();
    else if (++it == playerList.end())
        curPlayer = *playerList.begin();
    else
        curPlayer = *it;
    updatePlayer();
}

void GameWindow::ensureLord(int winner)
{
    QMessageBox::information(this, "提示", (winner == 1 ?
                                              PlayerName1 :
                                              winner == 2 ?
                                                  PlayerName2 :
                                                  PlayerName3) + QString::fromUtf8("成为地主"));
    curPlayer = isLord = winner;
    isLord == 1 ?
        player1PokerCount += 3 :
        isLord == 2 ?
            player2PokerCount += 3 :
            player3PokerCount += 3;
    QGraphicsView *views[3] = {ui->graphicsView, ui->graphicsView_2, ui->graphicsView_3};
    int            index    = 0;

    for (const auto &poker : vec1)
    {
        QGraphicsScene *scene = new QGraphicsScene(this);
        views[index]->setScene(scene);

        QString imagePath = QString(":/poker/%1.png").arg(poker);
        QPixmap pixMap(imagePath);

        if (pixMap.isNull())
        {
            qDebug() << "not find image/lord";
        }
        else
        {
            pixMap = pixMap.scaled(views[index]->size() * 0.9, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixMap);
            scene->setSceneRect(pixmapItem->boundingRect());
        }
        views[index]->setEnabled(true);
        views[index]->show();
        index ++;
        if (isLord == 1)
            player1.insert(poker);
        else if (isLord == 2)
            player2.insert(poker);
        else
            player3.insert(poker);
    }
    updatePlayer();
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
}

void GameWindow::updatePlayer()
{
    clearPoker();
    switch (curPlayer)
    {
        case 1 :
            ui->textEdit->setText(QString::fromUtf8("当前玩家：") + PlayerName1);
            ui->textEdit_7->setText(QString::number(player1PokerCount));
            ui->textEdit_3->setText(PlayerName2);
            ui->textEdit_4->setText(QString::number(player2PokerCount));
            ui->textEdit_5->setText(PlayerName3);
            ui->textEdit_6->setText(QString::number(player3PokerCount));
            break;
        case 2 :
            ui->textEdit->setText(QString::fromUtf8("当前玩家：") + PlayerName2);
            ui->textEdit_7->setText(QString::number(player2PokerCount));
            ui->textEdit_3->setText(PlayerName3);
            ui->textEdit_4->setText(QString::number(player3PokerCount));
            ui->textEdit_5->setText(PlayerName1);
            ui->textEdit_6->setText(QString::number(player1PokerCount));
            break;
        case 3 :
            ui->textEdit->setText(QString::fromUtf8("当前玩家：") + PlayerName3);
            ui->textEdit_7->setText(QString::number(player3PokerCount));
            ui->textEdit_3->setText(PlayerName1);
            ui->textEdit_4->setText(QString::number(player1PokerCount));
            ui->textEdit_5->setText(PlayerName2);
            ui->textEdit_6->setText(QString::number(player2PokerCount));
            break;
    }
}

void GameWindow::getLord()
{
    static int round = 1;
    if (sender() == ui->pushButton_3)
        lord.insert(curPlayer);
    ++lordRound;
    if (lordRound < playerList.size())
    {
        nextPlayer();
        return;
    }
    if (round == 1)
    {
        if (lord.empty())
        {
            ensureLord(1);
            return;
        }
        else if (lord.size() == 1)
        {
            ensureLord(*lord.begin());
            return;
        }
        else
        {
            playerList = lord;
            lord.clear();
            lordRound = 0;
            round     = 2;
            nextPlayer();
        }

    }
    else
        ensureLord(lord.empty() ?
                       curPlayer :
                       *lord.rbegin());
}

GameWindow::~GameWindow()
{
    delete ui;
}
