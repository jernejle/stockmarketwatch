#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H
#include <QTimer>
#include <QObject>
#include <QList>
#include <vector>
#include <string>
#include "datafetcher.h"
#include "stockdata.h"

class BackgroundWorker : public QObject
{
    Q_OBJECT

public:
    explicit BackgroundWorker(QString objName, QObject* parent = 0);

    std::vector<std::string> getSymbols() const;
    void setSymbols(const std::vector<std::string> &value);

signals:
    void valueChanged(QList<StockData>);

public slots:
    void update();
    void onTimerStopReceived();

private:
    QTimer* timer;
    DataFetcher datafetcher;
    std::vector<std::string> symbols;

};

Q_DECLARE_METATYPE(QList<StockData>)

#endif // BACKGROUNDWORKER_H
