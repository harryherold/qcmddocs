#pragma once

#include <QAbstractTableModel>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>

#include <note_collection.hpp>
#include <note_tree.hpp>

using Tag = QPair<QString, int>;

class QTextDocument;
class HeaderIterator;

inline SharedNoteTreePtr<Tag>
get_pointer(const QModelIndex &index)
{
    return static_cast<NoteTree<Tag> *>(index.internalPointer())->shared_from_this();
}

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit TreeModel(QObject *parent = nullptr);

    QVariant
    data(const QModelIndex &index, int role) const override;

    const Tag &
    data(const QModelIndex &index) const;

    Qt::ItemFlags
    flags(const QModelIndex &index) const override;

    QVariant
    headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex
    index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex
    parent(const QModelIndex &index) const override;

    int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int
    columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void
    setDocumentHeaders(SharedNoteTreePtr<Tag> tags);

  private:
    SharedNoteTreePtr<Tag> m_root;
};
