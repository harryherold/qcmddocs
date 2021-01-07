#pragma once

#include <memory>

#include <QByteArray>
#include <QString>
#include <QTextDocument>


#include <QPair>
#include <QVector>


#include <note_tree.hpp>

#include <markdownhighlighter.h>

class QTextDocument;
class HeaderIterator;

using Tag = QPair<QString, int>;

class NoteDocument
{
  public:
    explicit NoteDocument(const QString &file);
    ~NoteDocument() = default;
    QTextDocument &
    document();

    SharedNoteTreePtr<Tag>
    headers();

    void
    save();

  private:
    QTextDocument                        m_document;
    SharedNoteTreePtr<Tag>               m_headerTreeRoot;
    QString                              m_filePath;
    std::unique_ptr<MarkdownHighlighter> m_highlighter;
    QByteArray                           m_documentHash;

    void
    fillHeaderTree(SharedNoteTreePtr<Tag> root, HeaderIterator &header, int current_level = 1);

    void
    parseHeaders();
};
