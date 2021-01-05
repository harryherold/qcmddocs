#pragma once
#include <memory>
#include <optional>

#include <QCache>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextBlock>
#include <QTextDocument>
#include <QVector>


#include <note_document.hpp>

using NotePtr = NoteDocument *;

class NoteCollection
{
  public:
    NoteCollection(const QString &path);
    NoteCollection();

    void
    setCollectionPath(const QString &path);

    const QStringList &
    noteFiles() const;

    NotePtr
    getNote(const QString &file_path);

  private:
    QStringList                   m_files;
    QDir                          m_note_path;
    QCache<QString, NoteDocument> m_note_cache;
    static constexpr unsigned int m_cache_size = 32;

    inline void
    createFileList()
    {
        m_files = m_note_path.entryList(QStringList("*.md"), QDir::Files);
    }

    inline QString
    absoluteNotePath(const QString &file) const
    {
        return m_note_path.absolutePath() + '/' + file;
    }
};
