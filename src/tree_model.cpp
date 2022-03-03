#include <memory>

#include <QDebug>
#include <QPair>
#include <QTextDocument>
#include <QtAlgorithms>

#include <note_tree.hpp>
#include <tree_model.hpp>

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
}

void
TreeModel::setDocumentHeaders(SharedNoteTreePtr<Tag> tags)
{
    beginResetModel();
    m_root = tags;
    endResetModel();
}

QModelIndex
TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || m_root == nullptr)
    {
        return QModelIndex();
    }
    SharedNoteTreePtr<Tag> parentItem;
    if (!parent.isValid())
    {
        parentItem = m_root;
    }
    else
    {
        parentItem = get_pointer(parent);
    }

    auto childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem.get());
    }
    return QModelIndex();
}

QModelIndex
TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    auto item = get_pointer(index);
    Q_ASSERT(item != nullptr);

    if (item->parent().expired())
    {
        return QModelIndex();
    }

    auto parent = item->parent().lock();

    if (parent.get() == item.get() || !parent)
    {
        return QModelIndex();
    }
    return createIndex(parent->row(), 0, parent.get());
}

int
TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || m_root == nullptr)
    {
        return 0;
    }

    SharedNoteTreePtr<Tag> item = m_root;
    if (parent.isValid())
    {
        item = get_pointer(parent);
        Q_ASSERT(item != nullptr);
    }
    return item->childCount();
}

int
TreeModel::columnCount(const QModelIndex &parent) const
{
    if (m_root == nullptr)
    {
        return 0;
    }
    if (parent.isValid())
    {
        auto item = get_pointer(parent);
        Q_ASSERT(item != nullptr);
        return item->columnCount();
    }
    return m_root->columnCount();
}

QVariant
TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    auto item = get_pointer(index);
    Q_ASSERT(item != nullptr);
    return item->value().first;
}

const Tag &
TreeModel::data(const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    auto item = get_pointer(index);
    Q_ASSERT(item != nullptr);
    return item->value();
}

Qt::ItemFlags
TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant
TreeModel::headerData([[maybe_unused]] int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && m_root != nullptr)
    {
        return m_root->value().first;
    }
    return QVariant();
}
