#pragma once
#include <memory>

#include <QPair>
#include <QTextBlock>
#include <QTextDocument>
#include <QVector>

class HeaderIterator
{
  public:
    explicit HeaderIterator(const QTextDocument &document);

    bool
    isValid() const;

    int
    level() const;

    void
    setLevel(int level);

    HeaderIterator &
    operator++();

    HeaderIterator
    operator++(int);

    QString
    headerText() const;

    int
    position() const;

    void
    iterate();

  private:
    QTextBlock m_block_current{};
    int        m_level{0};
    QString    m_header_text;
    int        m_position{0};

    int
    headerLevel(const QString &s, int indent);
};
