#include <QApplication>
#include <QColumnView>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QIcon>
#include <QListView>
#include <QMenuBar>
#include <QSizePolicy>
#include <QStringListModel>
#include <QToolBar>
#include <QTreeView>
#include <QWidget>

#include <mainwindow.hpp>
#include <note_collection.hpp>
#include <note_document.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_listView(new QListView(this)), m_treeView(new QTreeView(this)),
      m_textEdit(new QTextEdit(this)), m_fileListModel(new QStringListModel(this))
{
    setupViews();
    setupLayout();
    setupMenu();
    setupConnects();
    setWindowTitle("QCmdDocs");
    resize({1200, 600});
}

void
MainWindow::setupViews()
{
    m_listView->setModel(m_fileListModel);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_treeModel = new TreeModel(this);
    m_treeView->setModel(m_treeModel);
}

#include <QSplitter>

void
MainWindow::setupLayout()
{
    QSizePolicy sp_left = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sp_left.setHorizontalStretch(1);
    m_listView->setSizePolicy(sp_left);
    m_treeView->setSizePolicy(sp_left);

    QSizePolicy sp_right = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sp_right.setHorizontalStretch(2);
    m_textEdit->setSizePolicy(sp_right);

    auto *splitterLeft = new QSplitter(this);
    splitterLeft->addWidget(m_listView);
    splitterLeft->addWidget(m_treeView);
    splitterLeft->setOrientation(Qt::Orientation::Horizontal);

    auto *splitterRight = new QSplitter(this);
    splitterRight->addWidget(splitterLeft);
    splitterRight->addWidget(m_textEdit);
    splitterLeft->setOrientation(Qt::Orientation::Vertical);

    setCentralWidget(splitterRight);
}

void
MainWindow::setupMenu()
{
    const QIcon open_icon(":/images/open.png");
    auto *      open_action = new QAction(open_icon, tr("&Cmddocs folder"), this);
    open_action->setShortcuts(QKeySequence::Open);
    open_action->setStatusTip(tr("Open docments folder"));
    connect(open_action, &QAction::triggered, this, &MainWindow::open);

    const QIcon new_file_icon(":/images/newfile.png");
    auto *      new_file_action = new QAction(new_file_icon, tr("&New file"), this);
    new_file_action->setShortcuts(QKeySequence::New);
    new_file_action->setToolTip(tr("New markdown file"));

    const QIcon save_note_icon(":/images/save.png");
    auto *      save_action = new QAction(save_note_icon, tr("&Save"), this);
    save_action->setShortcut(QKeySequence::Save);
    save_action->setToolTip(tr("Save"));
    connect(save_action, &QAction::triggered, this, &MainWindow::save);

    const QIcon exit_icon(":/images/exit.png");
    auto *      exit_action = new QAction(exit_icon, tr("&exit"), this);
    exit_action->setShortcut(QKeySequence::Quit);
    exit_action->setToolTip(tr("exit"));
    connect(exit_action, &QAction::triggered, this, []() { QApplication::quit(); });

    QToolBar *file_tool_bar = addToolBar(tr("File"));
    file_tool_bar->addAction(open_action);
    file_tool_bar->addAction(new_file_action);
    file_tool_bar->addAction(save_action);
    file_tool_bar->addAction(exit_action);
}

void
MainWindow::markEditorLine(const QTextCursor &cursor, QColor backgroundColor)
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection        selection;

    QColor lineColor = QColor(backgroundColor);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    m_textEdit->setExtraSelections(extraSelections);
}

void
MainWindow::setMarker(int pos)
{
    auto cursor = m_textEdit->textCursor();

    cursor.setPosition(pos);
    markEditorLine(cursor, QColor(Qt::yellow).lighter(160));
    m_textEdit->ensureCursorVisible();
    m_textEdit->setTextCursor(cursor);
}

void
MainWindow::unsetMarker()
{
    auto cursor = m_textEdit->textCursor();
    markEditorLine(cursor);
    cursor.setPosition(0);
    m_textEdit->setTextCursor(cursor);
}

void
MainWindow::setupConnects()
{
    connect(m_listView, &QListView::clicked, this, [this](const QModelIndex &index) {
        auto file_path    = m_fileListModel->data(index).toString();
        m_currentDocument = m_noteCollection.getNote(file_path);

        if (m_textEdit->document() != nullptr)
        {
            unsetMarker();
        }

        m_textEdit->setDocument(&m_currentDocument->document());
        m_treeModel->setDocumentHeaders(m_currentDocument->headers());
    });

    connect(m_treeView, &QTreeView::clicked, this, [this](const QModelIndex &index) {
        const Tag &t = m_treeModel->data(index);
        setMarker(t.second);
    });
}

void
MainWindow::open()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec())
    {
        auto paths = dialog.selectedFiles();
        m_notePath = paths.front();
        setWindowTitle(m_notePath);

        m_noteCollection.setCollectionPath(m_notePath);
        m_fileListModel->setStringList(m_noteCollection.noteFiles());
    }
}

void
MainWindow::save()
{
    m_currentDocument->save();
    m_treeModel->setDocumentHeaders(m_currentDocument->headers());
}
