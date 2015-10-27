#include "livestockwatchwindow.h"
#include "ui_livestockwatchwindow.h"
#include <iostream>

LiveStockWatchWindow::LiveStockWatchWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LiveStockWatchWindow)
{
    ui->setupUi(this);
}

LiveStockWatchWindow::~LiveStockWatchWindow()
{
    delete ui;
}

void LiveStockWatchWindow::closeEvent(QCloseEvent *event)
{
    emit stopTimer();
    this->thread->quit();
    this->thread->wait();
    this->thread->deleteLater();
    event->accept();
}

void LiveStockWatchWindow::onValueChanged(QList<StockData> val)
{
    for (StockData& s : val) {
        auto it = this->data.find(s.getSymbol());
        if (it != this->data.end()) {
            // QList for stock exists
            it->second.push_back(s);
        } else {
            // initialize new QList with new element
            this->data[s.getSymbol()] = QList<StockData>({s});
        }
    }

    this->graphPlotter.plotGraph(this->ui->centralPaintArea, this->data, this->colors);
    std::cout << "\n" << std::endl;
}
