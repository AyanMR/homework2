//
// Created by AyanMR on 25-6-26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GameWindow.h" resolved

#include "gamewindow.h"
#include "ui_GameWindow.h"


GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);

}

GameWindow::~GameWindow() {
    delete ui;
}
