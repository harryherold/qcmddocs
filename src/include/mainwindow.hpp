#pragma once

#include <QMainWindow>
#include <QTextEdit>

#include <note_collection.hpp>
#include <tree_model.hpp>

QT_BEGIN_NAMESPACE
class QListView;
class QTreeView;
class QMenu;
class QAction;
class QStringListModel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(const QString &notesPath, QWidget *parent = nullptr);

  public slots:
    void
    open();

    void
    save();

    void
    createNote();

    void
    adjustPointSizeBy(int pointSize);

  private:
    void
    setupViews();
    void
    setupLayout();
    void
    setupMenu();
    void
    setupConnects();

    void
    markEditorLine(const QTextCursor &cursor, QColor backgroundColor = Qt::white);

    void
    setMarker(int pos);

    void
    unsetMarker();

    void
    loadNotes(const QString &notesPath);

    QListView *       m_listView;
    QTreeView *       m_treeView;
    QTextEdit *       m_textEdit;
    QStringListModel *m_fileListModel;
    TreeModel *       m_treeModel;

    NoteCollection m_noteCollection;
    NotePtr        m_currentDocument;
};
