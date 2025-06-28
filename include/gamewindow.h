//
// Created by AyanMR on 25-6-26.
//

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <set>
#include <QDebug>
#include <QCheckBox>
#include <QGridLayout>
#include <QGraphicsPixmapItem>
extern QString PlayerName1;
extern QString PlayerName2;
extern QString PlayerName3;

QT_BEGIN_NAMESPACE

namespace Ui
{
    class GameWindow;
}

QT_END_NAMESPACE

class GameWindow : public QWidget
{
        Q_OBJECT

    public:
        explicit GameWindow(QWidget *parent = nullptr);

        bool eventFilter(QObject *watched, QEvent *event) override;

        ~GameWindow() override;

    public slots:
        void nextPlayer();

        void updatePlayer();

        void getLord();

        void clearPoker();

        void ensureLord(int winner);

    private:
        Ui::GameWindow *ui;
        std::set< int > lord;
        int             lordRound = 0;
        int             isLord    = 0;
        std::set< int > playerList;
        QGridLayout *   checkboxForPoker = nullptr;
        std::set< int > selectedCards;
};


#endif //GAMEWINDOW_H
