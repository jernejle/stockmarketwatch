#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablemodel.h"
#include <QSortFilterProxyModel>
#include <QListWidgetItem>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include "livestockwatchwindow.h"
#include <QThread>
#include "backgroundworker.h"
#include <iostream>
#include "backgrounddownloader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->dataFetcher = new DataFetcher();

    tm = new TableModel(this);
    sortFilter = new QSortFilterProxyModel(this);
    sortFilter->setSourceModel(tm);

    header = this->ui->tableView->horizontalHeader();
    header->setSortIndicator(1, Qt::AscendingOrder);

    ui->tableView->setModel(sortFilter);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();

    this->setPushButtonHandlers();
    connect(header, SIGNAL(sectionClicked(int)), this, SLOT(tableHeaderClicked(int)));

    // register meta type for transfering custom data over slots
    qRegisterMetaType<QList<StockData>>();
}

MainWindow::~MainWindow()
{
    delete this->sortFilter;
    delete this->tm;
    delete this->dataFetcher;
    delete ui;
}

void MainWindow::setPushButtonHandlers()
{
    connect(this->ui->refreshDataButton, SIGNAL (clicked()), this, SLOT (refreshDataHandler()));
    connect(this->ui->genGraphButton, SIGNAL (clicked()), this, SLOT (genGraphButtonClicked()));
    connect(this->ui->generateGraphByDate, SIGNAL (clicked()), this, SLOT (genGraphByDateButtonClicked()));
    connect(this->ui->liveTrackButton, SIGNAL(clicked()), this, SLOT(liveTrackButtonClicked()));
}

std::vector<std::string> MainWindow::selectedRows()
{
    QModelIndexList items = this->ui->tableView->selectionModel()->selectedIndexes();
    QStringList selected;
    std::vector<std::string> symb;

    // save all selected rows in QStringList
    for (int i = 0; i < items.count(); ++i) {
        const QModelIndex& qi = items.at(i);
        selected << qi.data(Qt::DisplayRole).toString();
    }

    // get only stock symbols
    for (int i = 1; i < selected.count(); i += 17) {
        symb.push_back(QString(selected.at(i)).toStdString());
    }

    return symb;
}

void MainWindow::tableHeaderClicked(int index)
{
    Qt::SortOrder order = this->header->sortIndicatorOrder();
    order = (order == Qt::DescendingOrder) ? Qt::AscendingOrder : Qt::DescendingOrder;
    this->sortFilter->sort(index, order);
}

void MainWindow::genGraphButtonClicked()
{
    std::time_t endDate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t startDate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() - std::chrono::hours(24*31));

    this->ui->dateEdit->setDate(QDateTime::fromTime_t(startDate).date());
    this->ui->dateEdit_2->setDate(QDateTime::fromTime_t(endDate).date());
    this->paintPlot(startDate, endDate);
}

void MainWindow::genGraphByDateButtonClicked()
{
    std::time_t startDate = QDateTime(this->ui->dateEdit->date()).toTime_t();
    std::time_t endDate = QDateTime(this->ui->dateEdit_2->date()).toTime_t();
    this->paintPlot(startDate, endDate);
}

void MainWindow::liveTrackButtonClicked()
{
    this->liveWatchWindow = new LiveStockWatchWindow();
    this->liveWatchWindow->setAttribute(Qt::WA_DeleteOnClose);
    this->liveWatchWindow->show();

    std::cout << "Main thread ";
    std::cout << QThread::currentThreadId() << std::endl;

    QThread* qthread = new QThread();
    this->liveWatchWindow->thread = qthread;
    qthread->start();

    BackgroundWorker* bg = new BackgroundWorker("");
    connect(bg, SIGNAL(valueChanged(QList<StockData>)), liveWatchWindow, SLOT(onValueChanged(QList<StockData>)));
    connect(liveWatchWindow, SIGNAL(stopTimer()), bg, SLOT(onTimerStopReceived()));
    bg->setSymbols(this->selectedRows());
    bg->moveToThread(qthread);
}

void MainWindow::paintPlot(time_t startDate, time_t endDate)
{
    std::vector<std::string> symb = this->selectedRows();
    if (symb.size() == 0)
        return;
    QList<QList<StockData>> history;
    this->dataFetcher->downloadHistoryFromYahoo(symb, history, startDate, endDate);
    this->graphPlotter.plotGraph(this->ui->graph1, history, startDate, endDate, this->ui->minMaxCheckbox->isChecked());
}


void MainWindow::refreshDataHandler()
{
    std::cout << "Main thread ";
    std::cout << QThread::currentThreadId() << std::endl;

    // clear existing data
    this->tm->resetModel();

    BackgroundDownloader* bgdl = new BackgroundDownloader;
    connect(this, SIGNAL(downloadStockDataInBg()), bgdl, SLOT(refreshStockData()));
    connect(&bgWorker, &QThread::finished, bgdl, &QObject::deleteLater);
    connect(bgdl, SIGNAL(processed()), this, SLOT(handleBackgroundDownloaderThread()));
    connect(bgdl, SIGNAL(stockDataReady(QList<StockData>)), this, SLOT(handleStockData(QList<StockData>)));

    bgdl->moveToThread(&bgWorker);
    this->bgWorker.start();
    emit downloadStockDataInBg();
}

void MainWindow::handleStockData(const QList<StockData> data)
{
    std::cout << data.size() << std::endl;
    this->tm->insertData(data);
}

void MainWindow::handleBackgroundDownloaderThread()
{
    this->bgWorker.quit();
    this->bgWorker.wait();
}
