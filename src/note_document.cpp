#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QPlainTextDocumentLayout>
#include <QString>
#include <QTextBlock>
#include <QTextStream>

#include <header_iterator.hpp>
#include <note_document.hpp>

QByteArray
createDocumentHash(const QTextDocument &document)
{
    return QCryptographicHash::hash(document.toPlainText().toUtf8(), QCryptographicHash::Md5);
}

NoteDocument::NoteDocument(const QString &file_path) : m_filePath(file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw std::runtime_error("The selected file could not be opened!");
    }

    QTextStream file_in(&file);
    m_document.setPlainText(file_in.readAll());

    m_headerTreeRoot = NoteTree<Tag>::create({QString("Navigation"), 0});

    parseHeaders();

    m_highlighter = std::make_unique<MarkdownHighlighter>(&m_document);

    m_documentHash = createDocumentHash(m_document);
}

QTextDocument &
NoteDocument::document()
{
    return m_document;
}

SharedNoteTreePtr<Tag>
NoteDocument::headers()
{
    return m_headerTreeRoot;
}

void
NoteDocument::save()
{
    auto currentHash = createDocumentHash(m_document);
    if (m_documentHash == currentHash)
    {
        qDebug() << "No changes since the last save/load.";
        return;
    }

    auto file = QFile{m_filePath};
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file " << m_filePath << " failed!";
        return;
    }

    QTextStream fileOut(&file);
    fileOut << m_document.toPlainText();
    file.close();

    m_headerTreeRoot->removeChilds();
    parseHeaders();
    m_documentHash = currentHash;
}

void
NoteDocument::fillHeaderTree(SharedNoteTreePtr<Tag> root, HeaderIterator &header, int current_level)
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
                fillHeaderTree(root->lastChild(), header, current_level + 1);
            }
        }
        else // header.level() <= current_level go down
        {
            break;
        }
    }
}

void
NoteDocument::parseHeaders()
{
    auto header = HeaderIterator(m_document);
    fillHeaderTree(m_headerTreeRoot, header);
}
