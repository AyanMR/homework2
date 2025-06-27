//
// Created by AyanMR on 25-6-26.
//

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QWidget {
Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override;

private:
    Ui::GameWindow *ui;
};


#endif //GAMEWINDOW_H
