#include "graphplotter.h"
#include <random>
#include <stdlib.h>
#include <iostream>

GraphPlotter::GraphPlotter()
{

}

void GraphPlotter::plotGraph(QCustomPlot *graph, QList<QList<StockData>> &data, std::time_t& startDate, std::time_t& endDate, bool minMax)
{
    std::srand(std::time(NULL));
    double seconds = 60*60*24;
    double difference = std::difftime(endDate,startDate) / (seconds);
    double tickStep = std::round(difference/31)+1;

    graph->clearPlottables();
    graph->xAxis->setDateTimeFormat("dd/MM");
    graph->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    graph->xAxis->setAutoTickStep(false);
    graph->xAxis->setTickStep(tickStep*seconds);
    graph->xAxis->setSubTickCount(2);
    graph->xAxis->setTickLabelFont(QFont(QFont().family(), 7));
    graph->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    int g = 0;

    for (int s = 0; s < data.count(); ++s) {
        QList<StockData>& lst = data[s];
        QVector<double> vals;
        QVector<double> x_date;
        QVector<double> max_vals;
        QVector<double> min_vals;
        graph->addGraph();

        QPen pen;
        QColor col(std::rand() % 255 + 1, std::rand() % 255 + 1, std::rand() % 255 + 1);
        pen.setWidth(2);
        pen.setColor(col);

        if (minMax) {
            graph->addGraph();
            graph->addGraph();

            QPen penMax;
            QColor col2(col.red()+(0.95*(255-col.red())), col.green()+(0.95*(255-col.green())), col.blue()+(0.95*(255-col.blue())));
            penMax.setStyle(Qt::DotLine);
            penMax.setColor(QColor(180,180,180));
            penMax.setWidth(1);
            graph->graph(g)->setPen(penMax);
            graph->graph(g)->setBrush(QBrush(col2));
            graph->graph(g)->setChannelFillGraph(graph->graph(g+1));
            graph->graph(g)->removeFromLegend();
            graph->graph(g+1)->setPen(penMax);
            graph->graph(g+1)->removeFromLegend();
            graph->graph(g+2)->setPen(pen);
        } else {
            graph->graph(g)->setPen(pen);
        }

        if (lst.count() != 0) {
            if (minMax) {
                graph->graph(g+2)->setName(lst[0].getSymbol().c_str());
            } else {
                graph->graph(g)->setName(lst[0].getSymbol().c_str());
            }
        }

        for (int val = 0; val < lst.count(); ++val) {
            StockData& ind = lst[val];
            vals.append(ind.getClose());
            x_date.append(ind.getDate());
            if (minMax) {
                max_vals.append(ind.getDaysHigh());
                min_vals.append(ind.getDaysLow());
            }
        }

        if (minMax) {
            graph->graph(g)->setData(x_date, max_vals);
            graph->graph(g+1)->setData(x_date, min_vals);
            graph->graph(g+2)->setData(x_date, vals);
        } else {
            graph->graph(g)->setData(x_date, vals);
        }
        g = (minMax) ? g + 3 : g + 1;
    }

    graph->rescaleAxes(true);
    graph->legend->setVisible(true);
    graph->legend->setFont(QFont(QFont().family(), 8));
    graph->xAxis->setRangeUpper(endDate);
    graph->replot();
}

void GraphPlotter::plotGraph(QCustomPlot *graph, std::map<std::string, QList<StockData>>& data, std::map<std::string, QColor>& colors)
{
    graph->clearPlottables();
    graph->xAxis->setDateTimeFormat("hh:mm");
    graph->xAxis->setAutoSubTicks(false);
    graph->xAxis->setSubTickCount(0);
    graph->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    graph->xAxis->setTickLabelFont(QFont(QFont().family(), 7));
    graph->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    int g = 0;
    for (auto it = data.begin(); it != data.end(); ++it) {
        QList<StockData>& lst = it->second;
        QVector<double> vals;
        QVector<double> x_date;
        graph->addGraph();

        QPen pen;
        QColor col;

        auto colit = colors.find(it->first);
        if (colit != colors.end()) {
            col = colit->second;
        } else {
            col.setRgb(std::rand() % 255 + 1, std::rand() % 255 + 1, std::rand() % 255 + 1);
            colors[it->first] = col;
        }

        pen.setWidth(2);
        pen.setColor(col);
        graph->graph(g)->setPen(pen);

        if (lst.count() != 0) {
            graph->graph(g)->setName(lst[0].getSymbol().c_str());
            for (int val = 0; val < lst.count(); ++val) {
                StockData& ind = lst[val];
                std::cout << ind.getSymbol() << std::endl;
                std::cout << ind.getTradePrice() << std::endl;
                vals.append(ind.getTradePrice());
                x_date.append(ind.getDate());
            }
            graph->graph(g)->setData(x_date, vals);
        }
        ++g;
    }

    graph->rescaleAxes();
    graph->xAxis->setAutoTickCount(20);
    graph->xAxis->setAutoTickStep(false);
    graph->xAxis->setTickStep(60);
    graph->legend->setVisible(true);
    graph->legend->setFont(QFont(QFont().family(), 8));
    graph->replot();
}

