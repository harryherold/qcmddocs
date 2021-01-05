#include <header_iterator.hpp>

HeaderIterator::HeaderIterator(const QTextDocument &document)
{
    m_markdownDocument = std::make_shared<QTextDocument>();
    m_markdownDocument->setMarkdown(document.toPlainText());
    m_block_current = m_markdownDocument->begin();
    iterate();
}

bool
HeaderIterator::isValid() const
{
    return m_block_current.isValid();
}

int
HeaderIterator::level() const
{
    return m_level;
}

void
HeaderIterator::setLevel(int level)
{
    m_level = level;
}

HeaderIterator &
HeaderIterator::operator++()
{
    m_block_current = m_block_current.next();
    iterate();
    return *this;
}

HeaderIterator
HeaderIterator::operator++(int)
{
    HeaderIterator hi{*this};
    this->         operator++();
    return hi;
}

QTextFragment
HeaderIterator::headerText() const
{
    return m_block_current.begin().fragment();
}

void
HeaderIterator::iterate()
{
    for (; m_block_current.isValid();)
    {
        int heading = m_block_current.blockFormat().headingLevel();
        if (heading > 0)
        {
            m_level = heading;
            break;
        }
        m_block_current = m_block_current.next();
    }
}
