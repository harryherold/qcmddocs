#include "header_iterator.h"
#include <QDebug>
#include <QFile>
#include <QTextDocument>


int
main(int argc, char *argv[])
{
    auto markdown_file = QFile("../note_example.md");
    if (!markdown_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file " << markdown_file.fileName();
        return 1;
    }
    QTextDocument doc;
    QTextStream   file_in(&markdown_file);
    doc.setMarkdown(file_in.readAll());

    for (auto iter = HeaderIterator(doc); iter.isValid(); ++iter)
    {
        auto f = iter.headerText();
        qDebug() << f.text() << " : " << f.position() << " level " << iter.level();
    }

    return 0;
}
