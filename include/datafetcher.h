#ifndef DATAFETCHER_H
#define DATAFETCHER_H
#include <string>
#include <QList>
#include "stockdata.h"
#include "document.h"
#include <chrono>
#include <time.h>

class DataFetcher
{
public:
    static std::vector<std::string> symbols;
    int symbOffset = 0;
    const int symbPerReq = 170;

    DataFetcher();
    void downloadLiveData(std::vector<std::string>& symbols, QList<StockData>& data);
    void downloadLiveData(QList<StockData>& data);
    void downloadData(std::string& url, std::string* pstr);
    void downloadHistoryFromYahoo(std::vector<std::string>& symbols, QList<QList<StockData> > &history, time_t &startDate, time_t &endDate);
    void prepareSymbols(std::vector<std::string>& symb);
    std::time_t convertDateStr(const std::string& dateStr);

private:
    void parseJson(std::string* str, QList<StockData>& plist);
    void parseJsonObject(const rapidjson::Value &tmp, StockData &sd);
    void parseJsonHistoryObj(std::string* str, QList<StockData>& plist);
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    void readStockSymbolsFromFile();
};

#endif // DATAFETCHER_H
