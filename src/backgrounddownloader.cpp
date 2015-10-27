#include "backgrounddownloader.h"
#include <QThread>
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>

BackgroundDownloader::BackgroundDownloader(QObject *parent) : QObject(parent)
{

}

void BackgroundDownloader::refreshStockData()
{
    std::cout << "refresing stock data in thread ";
    std::cout << QThread::currentThreadId() << std::endl;

    for (int i = 0; i < int(std::ceil(DataFetcher::symbols.size() / (double) this->dataFetcher.symbPerReq)); ++i) {
        QList<StockData> data;
        this->dataFetcher.downloadLiveData(data);
        emit stockDataReady(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    emit processed();
}

