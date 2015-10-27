#include "tablemodel.h"
#include <iostream>
#include "datafetcher.h"
#include <QSortFilterProxyModel>
#include <mainwindow.h>


TableModel::TableModel(QObject *parent): QAbstractTableModel(parent)
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return this->_data.count();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 17;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    const StockData& val = _data[index.row()];
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
        case 1: return val.getSymbol().c_str();
        case 2: return val.getName().c_str();
        case 3: return val.getStockExchange().c_str();
        case 4: return val.getTradePrice();
        case 5: return val.getAskPrice();
        case 6: return val.getBidPrice();
        case 7: return val.getChange();
        case 8: return val.getChangePercent().c_str();
        case 9: return val.getPrevClose();
        case 10: return val.getOpenPrice();
        case 11: return val.getDaysLow();
        case 12: return val.getDaysHigh();
        case 13: return val.getYearLow();
        case 14: return val.getYearHigh();
        case 15: return val.getPercentChangeFromYrLow().c_str();
        case 16: return val.getPercentChangeFromYrHigh().c_str();
        default: return QVariant();
        }
    } else if (role == Qt::DecorationRole) {
        QString iconstr;
        if (index.column() == 0) {
            if (val.getChange() > 0) {
                iconstr = ":/Images/res/images/index_up.png";
            } else if (val.getChange() < 0) {
                iconstr = ":/Images/res/images/index_down.png";
            }
            return QIcon(iconstr);
        }
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("");
            case 1:
                return QString("Symbol");
            case 2:
                return QString("Name");
            case 3:
                return QString("Stock Exchange");
            case 4:
                return QString("Trade Price");
            case 5:
                return QString("Ask");
            case 6:
                return QString("Bid");
            case 7:
                return QString("Change");
            case 8:
                return QString("Change %");
            case 9:
                return QString("Previous close");
            case 10:
                return QString("Open");
            case 11:
                return QString("Days Low");
            case 12:
                return QString("Days High");
            case 13:
                return QString("Year Low");
            case 14:
                return QString("Year High");
            case 15:
                return QString("Change % From Year Low");
            case 16:
                return QString("Change % From Year High");
            }
        }
    }
    return QVariant();
}

void TableModel::insertData(const QList<StockData> &data)
{
    for (int i = 0; i < data.count(); ++i) {
        const StockData& s = data[i];
        this->appendData(s);
    }
}

void TableModel::resetModel()
{
    this->beginResetModel();
    this->_data.clear();
    this->endResetModel();
}

void TableModel::appendData(const StockData& s)
{
    this->beginInsertRows(QModelIndex(), this->_data.count(), this->_data.count());
    this->_data.append(s);
    this->endInsertRows();
}

QList<StockData> &TableModel::getData()
{
    return this->_data;
}
