//
// Created by AyanMR on 25-6-27.
//

#include "gamewindow.h"
#include "ui_GameWindow.h"
#include "random.h"
#include "legalcheck.h"
std::set<int> vec1, player1, player2, player3;
std::set<int> curPlayerPoker;
int           lastPlayer        = 0;
int           curPlayer         = 0;
int           player1PokerCount = 17;
int           player2PokerCount = 17;
int           player3PokerCount = 17;


GameWindow::GameWindow(QWidget *parent) : QWidget(parent), ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    checkboxForPoker = new QGridLayout(ui->widget_2);
    ui->widget_2->setLayout(checkboxForPoker);
    std::vector<int> poker = genRanNum();
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
    nextPlayer(0);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &GameWindow::getLord);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &GameWindow::getLord);

    connect(ui->pushButton, &QPushButton::clicked, this, [this] {
        std::set<int> &currentPoker = curPlayer == 1 ?
                                          player1 :
                                          curPlayer == 2 ?
                                              player2 :
                                              player3;

        qDebug() << curPlayer << " sum:" << currentPoker.size();
        showCards(ui->widget_2, currentPoker, 8, 64, 100);
    });
    connect(ui->pushButton_4, &QPushButton::clicked, this, &GameWindow::useCards);
    connect(ui->pushButton_5, &QPushButton::clicked, this, [this] {
        if (lastPlayer != curPlayer && lastPlayer != 0)
        {
            selectedCards.clear();
            nextPlayer(1);
        }
        else
            QMessageBox::information(this, "提示", QString::fromUtf8("请出牌"));
    });
}

void GameWindow::useCards()
{
    if (isLegal(selectedCards))
    {
        for (const auto &card : selectedCards)
        {
            curPlayer == 1 ?
                player1.erase(card) :
                curPlayer == 2 ?
                    player2.erase(card) :
                    player3.erase(card);
        }
        curPlayer == 1 ?
            player1PokerCount -= selectedCards.size() :
            curPlayer == 2 ?
                player2PokerCount -= selectedCards.size() :
                player3PokerCount -= selectedCards.size();
        if ((curPlayer == 1 ?
                 player1PokerCount :
                 curPlayer == 2 ?
                     player2PokerCount :
                     player3PokerCount) == 0)
        {
            QMessageBox::information(this, "提示", (curPlayer == isLord ?
                                                      QString::fromUtf8("地主") :
                                                      QString::fromUtf8("农民")) + (curPlayer == 1 ?
                                                     PlayerName1 :
                                                     curPlayer == 2 ?
                                                         PlayerName2 :
                                                         PlayerName3) + QString::fromUtf8("获胜"));
            exit(0);
        }
        else
            nextPlayer(1);
    }
    else
    {
        QMessageBox::information(this, "提示", QString::fromUtf8("不合法的牌型"));
        for (auto i : selectedCards)
        {
            qDebug().noquote().nospace() << i << " ";
        }
    }
}


void GameWindow::clearPoker()
{
    selectedCards.clear();
    QLayoutItem *ch;
    while ((ch = checkboxForPoker->takeAt(0)) != nullptr)
    {
        delete ch->widget();
        delete ch;
    }
    // ui->widget_2->updateGeometry();
    // ui->widget_2->adjustSize();
    // this->adjustSize();
};

bool GameWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QLabel *cardLabel = qobject_cast<QLabel *>(watched);
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
                cardLabel->setStyleSheet("border: 2px solid red; padding: 0px;");
            }

            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}


void GameWindow::nextPlayer(bool isShow)
{
    usedCards2 = usedCards1;
    usedCards1 = selectedCards;
    auto it    = playerList.find(curPlayer);
    if (it == playerList.end())
        curPlayer = *playerList.begin();
    else if (++it == playerList.end())
        curPlayer = *playerList.begin();
    else
        curPlayer = *it;
    updatePlayer(isShow);
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
            pixMap = pixMap.scaled(views[index]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            views[index]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            views[index]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    updatePlayer(0);
    ui->textEdit_2->setText(QString::fromUtf8("地主是：") + (isLord == 1 ?
                                                             PlayerName1 :
                                                             isLord == 2 ?
                                                                 PlayerName2 :
                                                                 PlayerName3));
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
}

void GameWindow::updatePlayer(bool isShow)
{
    clearPoker();
    if (isShow)
    {
        showCards(ui->widget_6, usedCards2, 4, 64, 100);
        showCards(ui->widget_7, usedCards1, 4, 64, 100);
    }
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
        nextPlayer(0);
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
            nextPlayer(0);
        }
    }
    else
        ensureLord(lord.empty() ?
                       curPlayer :
                       *lord.rbegin());
}

void GameWindow::showCards(QWidget *widget, const std::set<int> &cards, int count, int weight, int height)
{
    QGridLayout *layout = nullptr;
    if (widget == ui->widget_2)
    {
        layout = checkboxForPoker;
    }
    else
    {
        if (widget->layout())
        {
            QLayoutItem *item;
            while ((item = widget->layout()->takeAt(0)) != nullptr)
            {
                if (item->widget())
                {
                    delete item->widget();
                }
                delete item;
            }
            delete widget->layout();
        }
        layout = new QGridLayout(widget);
        widget->setLayout(layout);
    }

    int col = 0;
    int row = 0;
    for (auto poker : cards)
    {
        QString imagePath = QString(":/poker/%1.png").arg(poker);
        QLabel *label     = new QLabel(widget);
        label->setText(QString::number(poker));
        label->setMargin(0);
        label->setContentsMargins(0, 0, 0, 0);

        QPixmap pixMap(imagePath);
        if (pixMap.isNull())
            qDebug() << "not find image";
        else
        {
            pixMap = pixMap.scaled(weight, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label->setPixmap(pixMap);
            label->setFixedSize(pixMap.size());
        }
        label->setProperty("value", poker);
        label->setFrameShape(QFrame::NoFrame);
        label->setLineWidth(0);
        label->setStyleSheet("border: 2px solid transparent; padding: 0px;");
        if (widget == ui->widget_2)
        {
            label->installEventFilter(this);
        }

        layout->addWidget(label, row, col ++);
        if (col >= count)
        {
            col = 0;
            row ++;
        }
    }
    // widget->adjustSize();
}


GameWindow::~GameWindow()
{
    delete ui;
}
