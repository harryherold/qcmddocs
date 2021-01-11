#include "note_collection.hpp"

#include <QDebug>
#include <QString>

int
main()
{
    QString        path = "/home/cherold/Documents/cmddocs";
    QString        file = "cpp.md";
    NoteCollection nc(path);
    auto *         n = nc.getNote(file);
    qDebug() << "Found Tags:";
    for (const auto &t : n->tags())
    {
        qDebug() << t;
    }
    return 0;
}
