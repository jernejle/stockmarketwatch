#include "backgroundworker.h"
#include <QThread>
#include <QList>
#include <iostream>
#include <stockdata.h>

BackgroundWorker::BackgroundWorker(QString objName, QObject *parent) :
    QObject(parent)
{
    this->setObjectName(objName);
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    this->timer->start(60000);
}

void BackgroundWorker::update()
{
    std::cout << "Background thread ";
    std::cout << QThread::currentThreadId() << std::endl;
    QList<StockData> data;
    this->datafetcher.downloadLiveData(this->symbols, data);
    emit valueChanged(data);
}

void BackgroundWorker::onTimerStopReceived()
{
    this->timer->stop();
    delete this;
}
std::vector<std::string> BackgroundWorker::getSymbols() const
{
    return symbols;
}

void BackgroundWorker::setSymbols(const std::vector<std::string> &value)
{
    symbols = value;
    this->datafetcher.prepareSymbols(symbols);
    this->update();
}

