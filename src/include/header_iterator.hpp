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

    QTextFragment
    headerText() const;

    int
    position() const;

    void
    iterate();

  private:
    std::shared_ptr<QTextDocument> m_markdownDocument;
    QTextBlock                     m_block_current{};
    int                            m_level{0};
};
