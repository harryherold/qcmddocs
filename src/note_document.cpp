#include <QDebug>
#include <QFile>
#include <QPlainTextDocumentLayout>
#include <QString>
#include <QTextBlock>
#include <QTextDocumentWriter>
#include <QTextStream>

#include <header_iterator.hpp>
#include <note_document.hpp>

NoteDocument::NoteDocument(const QString &file_path) : m_filePath(file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw std::runtime_error("The selected file could not be opened!");
    }

    QTextStream file_in(&file);
    m_document.setPlainText(file_in.readAll());
    m_root = NoteTree<Tag>::create({QString("Navigation"), 0});

    auto header = HeaderIterator(m_document);
    fillTree(m_root, header);

    m_highlighter = std::make_unique<MarkdownHighlighter>(&m_document);
}

QTextDocument &
NoteDocument::document()
{
    return m_document;
}

SharedNoteTreePtr<Tag>
NoteDocument::tags()
{
    return m_root;
}

void
NoteDocument::save() const
{
    // TODO re-create tag tree
    QTextDocumentWriter document_writer(m_filePath);
    auto                ret = document_writer.write(&m_document);
    if (!ret)
    {
        qDebug() << "Write to " << m_filePath << " failed";
    }
}

void
NoteDocument::fillTree(SharedNoteTreePtr<Tag> root, HeaderIterator &header, int current_level)
{
    Q_ASSERT(root != nullptr);

    while (header.isValid())
    {
        if (header.level() == current_level)
        {
            root->appendChild(Tag{header.headerText(), header.position()});
            ++header;
        }
        else if (header.level() > current_level)
        {
            if (root->childCount() == 0)
            {
                // broken structure
                current_level = header.level();
            }
            else
            {
                // next level
                fillTree(root->lastChild(), header, current_level + 1);
            }
        }
        else // header.level() <= current_level go down
        {
            break;
        }
    }
}
