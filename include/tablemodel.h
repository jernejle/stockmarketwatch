#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QAbstractTableModel>
#include "stockdata.h"
#include <QList>
#include "datafetcher.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void insertData(const QList<StockData> &data);
    void resetModel();
    void appendData(const StockData& s);
    QList<StockData>& getData();

private:
    QList<StockData> _data;
    DataFetcher fetcher;

};

#endif // TABLEMODEL_H
