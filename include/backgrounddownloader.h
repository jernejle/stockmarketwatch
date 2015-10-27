#ifndef BACKGROUNDDOWNLOADER_H
#define BACKGROUNDDOWNLOADER_H

#include <QObject>
#include <QList>
#include <QThread>
#include "stockdata.h"
#include "datafetcher.h"

class BackgroundDownloader : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundDownloader(QObject *parent = 0);

signals:
    void stockDataReady(const QList<StockData> data);
    void processed();

public slots:
    void refreshStockData();

private:
    DataFetcher dataFetcher;
    QThread* thread;

};

#endif // BACKGROUNDDOWNLOADER_H
