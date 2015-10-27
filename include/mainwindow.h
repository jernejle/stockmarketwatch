#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QThread>
#include <QList>
#include <vector>
#include "datafetcher.h"
#include "stockdata.h"
#include "tablemodel.h"
#include "graphplotter.h"
#include "livestockwatchwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setPushButtonHandlers();

private:
    Ui::MainWindow *ui;
    LiveStockWatchWindow* liveWatchWindow;
    TableModel *tm;
    DataFetcher *dataFetcher;
    QSortFilterProxyModel *sortFilter;
    GraphPlotter graphPlotter;
    QHeaderView* header;
    QThread bgWorker;


    std::vector<std::string> selectedRows();
    void paintPlot(std::time_t startDate, std::time_t endDate);


private slots:
    void refreshDataHandler();
    void tableHeaderClicked(int index);
    void genGraphButtonClicked();
    void genGraphByDateButtonClicked();
    void liveTrackButtonClicked();

public slots:
    void handleStockData(const QList<StockData> data);
    void handleBackgroundDownloaderThread();

signals:
    void downloadStockDataInBg();

};

#endif // MAINWINDOW_H
