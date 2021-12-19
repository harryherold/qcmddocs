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
    MainWindow(QWidget *parent = nullptr);

  public slots:
    void
    open();

    void
    save();

    void
    createNote();

    void
    increaseFontSize();

  private:
    QListView *       m_listView;
    QTreeView *       m_treeView;
    QTextEdit *       m_textEdit;
    QStringListModel *m_fileListModel;
    TreeModel *       m_treeModel;

    QString        m_notePath;
    NoteCollection m_noteCollection;
    NotePtr        m_currentDocument;

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
};
