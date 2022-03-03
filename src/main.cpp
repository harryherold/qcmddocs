#include <QApplication>
#include <QFile>
#include <mainwindow.hpp>

QString
getNotesPath(int argc, char *argv[])
{
    if (argc == 2)
    {
        auto fileInfo = QFileInfo(argv[1]);
        if (fileInfo.exists() && fileInfo.isDir())
        {
            return argv[1];
        }
    }
    return "";
}

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto         notesPath = getNotesPath(argc, argv);
    MainWindow   w{notesPath};
    w.show();
    return a.exec();
}
