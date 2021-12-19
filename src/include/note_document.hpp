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

class NoteHighlighter : public MarkdownHighlighter
{
  public:
    NoteHighlighter(QTextDocument *     parent              = nullptr,
                    HighlightingOptions highlightingOptions = HighlightingOption::None);

    static void
    textFormatFor(std::function<void(QTextCharFormat &)> formatCallback);
};

class NoteDocument
{

  public:
    enum class Mode
    {
        Open,
        Create
    };

    explicit NoteDocument(const QString &file, Mode mode = Mode::Open);
    ~NoteDocument() = default;
    QTextDocument &
    document();

    SharedNoteTreePtr<Tag>
    headers();

    void
    save();

    NoteHighlighter *
    highlighter()
    {
        return m_highlighter.get();
    }

  private:
    QTextDocument                    m_document;
    SharedNoteTreePtr<Tag>           m_headerTreeRoot;
    QString                          m_filePath;
    std::unique_ptr<NoteHighlighter> m_highlighter;
    QByteArray                       m_documentHash;

    void
    fillHeaderTree(SharedNoteTreePtr<Tag> root, HeaderIterator &header, int current_level = 1);

    void
    parseHeaders();

    void
    openDocument();

    void
    createDocument();
};
