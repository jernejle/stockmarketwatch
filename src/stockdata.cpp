#include "stockdata.h"

StockData::StockData()
{

}
std::string StockData::getPercentChangeFromYrHigh() const
{
    return percentChangeFromYrHigh;
}

void StockData::setPercentChangeFromYrHigh(std::string value)
{
    percentChangeFromYrHigh = value;
}
std::string StockData::getPercentChangeFromYrLow() const
{
    return percentChangeFromYrLow;
}

void StockData::setPercentChangeFromYrLow(std::string value)
{
    percentChangeFromYrLow = value;
}
double StockData::getYearHigh() const
{
    return yearHigh;
}

void StockData::setYearHigh(double value)
{
    yearHigh = value;
}
double StockData::getYearLow() const
{
    return yearLow;
}

void StockData::setYearLow(double value)
{
    yearLow = value;
}
double StockData::getDaysHigh() const
{
    return daysHigh;
}

void StockData::setDaysHigh(double value)
{
    daysHigh = value;
}
double StockData::getDaysLow() const
{
    return daysLow;
}

void StockData::setDaysLow(double value)
{
    daysLow = value;
}
double StockData::getOpenPrice() const
{
    return openPrice;
}

void StockData::setOpenPrice(double value)
{
    openPrice = value;
}
double StockData::getPrevClose() const
{
    return prevClose;
}

void StockData::setPrevClose(double value)
{
    prevClose = value;
}
std::string StockData::getChangePercent() const
{
    return changePercent;
}

void StockData::setChangePercent(std::string value)
{
    changePercent = value;
}
double StockData::getChange() const
{
    return change;
}

void StockData::setChange(double value)
{
    change = value;
}
double StockData::getBidPrice() const
{
    return bidPrice;
}

void StockData::setBidPrice(double value)
{
    bidPrice = value;
}
double StockData::getAskPrice() const
{
    return askPrice;
}

void StockData::setAskPrice(double value)
{
    askPrice = value;
}
double StockData::getTradePrice() const
{
    return tradePrice;
}

void StockData::setTradePrice(double value)
{
    tradePrice = value;
}
std::string StockData::getStockExchange() const
{
    return stockExchange;
}

void StockData::setStockExchange(const std::string &value)
{
    stockExchange = value;
}
std::string StockData::getName() const
{
    return name;
}

void StockData::setName(const std::string &value)
{
    name = value;
}
std::string StockData::getSymbol() const
{
    return symbol;
}

void StockData::setSymbol(const std::string &value)
{
    symbol = value;
}
time_t StockData::getDate() const
{
    return date;
}

void StockData::setDate(const time_t &value)
{
    date = value;
}
double StockData::getClose() const
{
    return close;
}

void StockData::setClose(double value)
{
    close = value;
}



















