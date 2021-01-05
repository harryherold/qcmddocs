#pragma once

#include <memory>

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
    tags();

    void
    save() const;

  private:
    QTextDocument                        m_document;
    SharedNoteTreePtr<Tag>               m_root;
    QString                              m_filePath;
    std::unique_ptr<MarkdownHighlighter> m_highlighter;

    void
    fillTree(SharedNoteTreePtr<Tag> root, HeaderIterator &header, int current_level = 1);
};
