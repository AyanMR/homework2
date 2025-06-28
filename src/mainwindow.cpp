//
// Created by AyanMR on 25-6-26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "gamewindow.h"
#include "ui_GameWindow.h"
#include <QMessageBox>
QString PlayerName1, PlayerName2, PlayerName3;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "TEST";
        PlayerName1 = ui->textEdit->toPlainText();
        PlayerName2 = ui->textEdit_2->toPlainText();
        PlayerName3 = ui->textEdit_3->toPlainText();
        if (!PlayerName1.isEmpty() && !PlayerName2.isEmpty() && !PlayerName3.isEmpty())
        {
            if (PlayerName1 == PlayerName2 || PlayerName1 == PlayerName3 || PlayerName2 == PlayerName3)
            {
                QMessageBox::warning(this, "警告", "玩家姓名不能重复");
                return;
            }
            GameWindow *gameWindow = new GameWindow();
            gameWindow->show();
            this->close();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
