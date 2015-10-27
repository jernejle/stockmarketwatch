#include <QFile>
#include <QTextStream>
#include "datafetcher.h"
#include "curl_easy.h"
#include <string>
#include <iostream>
#include <fstream>
#include "document.h"
#include "stockdata.h"
#include <chrono>
#include <vector>

std::vector<std::string> DataFetcher::symbols;

DataFetcher::DataFetcher()
{
    if (DataFetcher::symbols.empty()) {
        this->readStockSymbolsFromFile();
    }
}

size_t DataFetcher::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size*nmemb);
    return size*nmemb;
}

void DataFetcher::readStockSymbolsFromFile()
{
    QFile file(":/Files/symbols");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        DataFetcher::symbols.push_back(line.toStdString());
    }

    file.close();
}

void DataFetcher::downloadLiveData(std::vector<std::string> &symbols, QList<StockData> &data)
{
    std::string startU = "http://query.yahooapis.com/v1/public/yql?q=";
    std::string yql = "select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(";
    std::string endU = "&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";
    std::string url = yql;

    std::string yhoo_url = startU;

    if (this->symbOffset >= symbols.size()) {
        this->symbOffset = 0;
    }

    std::cout << "Downloading " << this->symbOffset+170 << std::endl;

    for (int i = this->symbOffset; i < symbols.size(); ++i) {
        std::string& s = symbols.at(i);
        if (i == this->symbOffset + this->symbPerReq || s == symbols.back()) {
            url.append(s).append(")").append(endU);
            yhoo_url.append(url);
            break;
        } else {
            url.append(s).append(",");
        }
    }

    this->symbOffset += this->symbPerReq;

    std::string str;
    this->downloadData(yhoo_url, &str);
    this->parseJson(&str, data);
}

void DataFetcher::downloadLiveData(QList<StockData> &data)
{
    this->downloadLiveData(DataFetcher::symbols, data);
}

void DataFetcher::downloadData(std::string& url, std::string *pstr)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataFetcher::WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pstr);

    try {
        res = curl_easy_perform(curl);
    } catch (curl_easy_exception error) {
        // If you want to get the entire error stack we can do:
        vector<pair<string,string>> errors = error.get_traceback();
        // Otherwise we could print the stack like this:
        error.print_traceback();
        // Note that the printing the stack will erase it
    }
    curl_easy_cleanup(curl);
}

void DataFetcher::downloadHistoryFromYahoo(std::vector<std::string>& symbols, QList<QList<StockData>>& history, std::time_t& startDate, std::time_t& endDate)
{
    char startDtChr[16];
    char endDtChr[16];

    std::tm* st_ptr = std::localtime(&startDate);
    std::strftime(startDtChr, sizeof(startDtChr), "%Y-%m-%d", st_ptr);
    std::tm* end_ptr = std::localtime(&endDate);
    std::strftime(endDtChr, sizeof(endDtChr), "%Y-%m-%d", end_ptr);

    std::string yhoo_url = "http://query.yahooapis.com/v1/public/yql?q=";
    std::string yql1 = "select%20*%20from%20yahoo.finance.historicaldata%20where%20symbol%20%3D%20";
    std::string yqlUrl = yql1;
    std::string url = yhoo_url;
    std::string jsonDta;

    for (unsigned int i = 0; i < symbols.size(); ++i) {
        yqlUrl += "%22" + symbols.at(i) + "%22%20and%20";
        yqlUrl += "startDate%20%3D%20%22" + std::string(startDtChr) + "%22%20and%20endDate%20%3D%20%22" + std::string(endDtChr) + "%22&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&callback=";
        url += yqlUrl;
        QList<StockData> tmp;
        this->downloadData(url, &jsonDta);
        this->parseJsonHistoryObj(&jsonDta, tmp);
        yqlUrl = yql1;
        url = yhoo_url;
        history.append(tmp);
    }
}

void DataFetcher::prepareSymbols(std::vector<std::string> &symb)
{
    for (std::string& s : symb) {
        s.insert(0, 1, '"');
        s += '"';
    }
}

void DataFetcher::parseJson(std::string *str, QList<StockData> &plist)
{
    if (str == nullptr || str->empty())
        return;

    rapidjson::Document c;
    if (c.Parse(str->c_str()).HasParseError() || !c.HasMember("query") || c["query"]["count"].GetInt() == 0)
        return;

    const rapidjson::Value& results = c["query"]["results"];
    const rapidjson::Value& quotes = results["quote"];

    if (quotes.IsArray()) {
        for (rapidjson::SizeType i = 0; i < quotes.Size(); ++i) {
            StockData sd;
            this->parseJsonObject(quotes[i], sd);
            plist.append(sd);
        }
    } else {
        StockData sd;
        parseJsonObject(quotes, sd);
        plist.append(sd);
    }
    return;
}

void DataFetcher::parseJsonObject(const rapidjson::Value& tmp, StockData& sd)
{
    if (!tmp["symbol"].IsNull())
        sd.setSymbol(tmp["symbol"].GetString());
    if (!tmp["Name"].IsNull())
        sd.setName(tmp["Name"].GetString());
    if (!tmp["StockExchange"].IsNull())
        sd.setStockExchange(tmp["StockExchange"].GetString());
    if (!tmp["LastTradePriceOnly"].IsNull())
        sd.setTradePrice(std::strtod(tmp["LastTradePriceOnly"].GetString(), nullptr));
    if (!tmp["Ask"].IsNull())
        sd.setAskPrice(std::strtod(tmp["Ask"].GetString(), nullptr));
    if (!tmp["Bid"].IsNull())
        sd.setBidPrice(std::strtod(tmp["Bid"].GetString(), nullptr));
    if (!tmp["Change"].IsNull())
        sd.setChange(std::strtod(tmp["Change"].GetString(), nullptr));
    if (!tmp["ChangeinPercent"].IsNull())
        sd.setChangePercent(tmp["ChangeinPercent"].GetString());
    if (!tmp["PreviousClose"].IsNull())
        sd.setPrevClose(std::strtod(tmp["PreviousClose"].GetString(), nullptr));
    if (!tmp["Open"].IsNull())
        sd.setOpenPrice(std::strtod(tmp["Open"].GetString(), nullptr));
    if (!tmp["DaysLow"].IsNull())
        sd.setDaysLow(std::strtod(tmp["DaysLow"].GetString(), nullptr));
    if (!tmp["DaysHigh"].IsNull())
        sd.setDaysHigh(std::strtod(tmp["DaysHigh"].GetString(), nullptr));
    if (!tmp["YearLow"].IsNull())
        sd.setYearLow(std::strtod(tmp["YearLow"].GetString(), nullptr));
    if (!tmp["YearHigh"].IsNull())
        sd.setYearHigh(std::strtod(tmp["YearHigh"].GetString(), nullptr));
    if (!tmp["PercentChangeFromYearLow"].IsNull())
        sd.setPercentChangeFromYrLow(tmp["PercentChangeFromYearLow"].GetString());
    if (!tmp["PercebtChangeFromYearHigh"].IsNull())
        sd.setPercentChangeFromYrHigh(tmp["PercebtChangeFromYearHigh"].GetString());

    sd.setDate(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

void DataFetcher::parseJsonHistoryObj(std::string *str, QList<StockData> &plist)
{
    if (str == nullptr || str->empty())
        return;

    rapidjson::Document c;
    if (c.Parse(str->c_str()).HasParseError() || !c.HasMember("query") || c["query"]["count"].GetInt() == 0)
        return;

    const rapidjson::Value& results = c["query"]["results"];

    if (results.IsNull())
        return;

    const rapidjson::Value& quotes = results["quote"];
    for (rapidjson::SizeType i = 0; i < quotes.Size(); ++i) {
        const rapidjson::Value& tmp = quotes[i];
        StockData sd;
        if (!tmp["Symbol"].IsNull())
            sd.setSymbol(tmp["Symbol"].GetString());
        if (!tmp["Date"].IsNull())
            sd.setDate(this->convertDateStr(tmp["Date"].GetString()));
        if (!tmp["High"].IsNull())
            sd.setDaysHigh(std::strtod(tmp["High"].GetString(), nullptr));
        if (!tmp["Low"].IsNull())
            sd.setDaysLow(std::strtod(tmp["Low"].GetString(), nullptr));
        if (!tmp["Close"].IsNull())
            sd.setClose(std::strtod(tmp["Close"].GetString(), nullptr));
        plist.append(sd);
    }
    *str = "";
}

time_t DataFetcher::convertDateStr(const std::string& dateStr)
{
    // input "2010-03-02"
    int year = std::stoi(dateStr.substr(0,4));
    int mon = std::stoi(dateStr.substr(5,2));
    int day = std::stoi(dateStr.substr(8,2));

    std::tm tm;
    time_t rawtime;
    std::time(&rawtime);
    tm = *std::localtime(&rawtime);
    tm.tm_year = year - 1900;
    tm.tm_mon = mon - 1;
    tm.tm_mday = day;
    return mktime(&tm);
}

