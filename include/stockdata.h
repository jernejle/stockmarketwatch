#ifndef STOCKDATA_H
#define STOCKDATA_H
#include <string>
#include <chrono>

class StockData
{
public:
    StockData();

    std::string getPercentChangeFromYrHigh() const;
    void setPercentChangeFromYrHigh(std::string value);

    std::string getPercentChangeFromYrLow() const;
    void setPercentChangeFromYrLow(std::string value);

    double getYearHigh() const;
    void setYearHigh(double value);

    double getYearLow() const;
    void setYearLow(double value);

    double getDaysHigh() const;
    void setDaysHigh(double value);

    double getDaysLow() const;
    void setDaysLow(double value);

    double getOpenPrice() const;
    void setOpenPrice(double value);

    double getPrevClose() const;
    void setPrevClose(double value);

    std::string getChangePercent() const;
    void setChangePercent(std::string value);

    double getChange() const;
    void setChange(double value);

    double getBidPrice() const;
    void setBidPrice(double value);

    double getAskPrice() const;
    void setAskPrice(double value);

    double getTradePrice() const;
    void setTradePrice(double value);

    std::string getStockExchange() const;
    void setStockExchange(const std::string &value);

    std::string getName() const;
    void setName(const std::string &value);

    std::string getSymbol() const;
    void setSymbol(const std::string &value);

    std::time_t getDate() const;
    void setDate(const std::time_t &value);

    double getClose() const;
    void setClose(double value);

private:
    std::string symbol;
    std::string name;
    std::string stockExchange;
    std::time_t date;
    double tradePrice = 0.0;
    double askPrice = 0.0;
    double bidPrice = 0.0;
    double change = 0.0;
    std::string changePercent;
    double prevClose = 0.0;
    double close = 0;
    double openPrice = 0.0;
    double daysLow = 0.0;
    double daysHigh = 0.0;
    double yearLow = 0.0;
    double yearHigh = 0.0;
    std::string percentChangeFromYrLow;
    std::string percentChangeFromYrHigh;
};

#endif // STOCKDATA_H
