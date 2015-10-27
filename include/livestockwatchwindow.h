#ifndef LIVESTOCKWATCHWINDOW_H
#define LIVESTOCKWATCHWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QThread>
#include <QColor>
#include <string>
#include <map>
#include "graphplotter.h"
#include "stockdata.h"

namespace Ui {
class LiveStockWatchWindow;
}

class LiveStockWatchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LiveStockWatchWindow(QWidget *parent = 0);
    ~LiveStockWatchWindow();
    QThread* thread;

signals:
    void stopTimer();

private:
    Ui::LiveStockWatchWindow *ui;
    GraphPlotter graphPlotter;
    std::map<std::string, QList<StockData>> data;
    std::map<std::string, QColor> colors;

    void closeEvent(QCloseEvent *event);

public slots:
    void onValueChanged(QList<StockData>);
};

#endif // LIVESTOCKWATCHWINDOW_H
