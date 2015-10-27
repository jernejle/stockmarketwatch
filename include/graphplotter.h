#ifndef GRAPHPLOTTER_H
#define GRAPHPLOTTER_H
#include "lib/qcustomplot/qcustomplot.h"
#include <QList>
#include "stockdata.h"
#include <chrono>
#include <datafetcher.h>

class GraphPlotter
{
public:
    GraphPlotter();
    void plotGraph(QCustomPlot* graph, QList<QList<StockData>>& data, time_t& startDate, time_t& endDate, bool minMax);
    void plotGraph(QCustomPlot* graph, std::map<std::string, QList<StockData> > &data, std::map<std::string, QColor> &colors);
};

#endif // GRAPHPLOTTER_H
