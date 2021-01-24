#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextStream>

#include <note_collection.hpp>
#include <note_document.hpp>

NoteCollection::NoteCollection(const QString &path) : m_note_path(path), m_note_cache(m_cache_size)
{
    createFileList();
}

NoteCollection::NoteCollection() : m_note_cache(m_cache_size)
{
}

void
NoteCollection::setCollectionPath(const QString &path)
{
    m_note_path = QDir(path);
    createFileList();
}

const QStringList &
NoteCollection::noteFiles() const
{
    return m_files;
}

NotePtr
NoteCollection::getNote(const QString &file_path)
{
    if (!m_note_cache.contains(file_path))
    {
        m_note_cache.insert(file_path, new NoteDocument(absoluteNotePath(file_path)));
    }
    return m_note_cache[file_path];
}

NotePtr
NoteCollection::createNote(const QString &file_path)
{
    if (m_note_cache.contains(file_path))
    {
        throw std::runtime_error("Note file already exists!");
    }
    m_files.append(file_path);
    auto file_info = QFileInfo{m_note_path, file_path};
    m_note_cache.insert(file_path, new NoteDocument(file_info.absoluteFilePath(), NoteDocument::Mode::Create));
    return m_note_cache[file_path];
}
