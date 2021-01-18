#include <header_iterator.hpp>

#include <QStringRef>

inline int
count_char(const QString &s, QLatin1Char character, int offset = 0)
{
    int count = 0;
    for (int i = offset; i < s.count(); i++)
    {
        if (s[i] == character)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

inline bool
contains_only(const QString &s, QLatin1Char character)
{
    return count_char(s, character) == s.count();
}

int
HeaderIterator::headerLevel(const QString &s, int indent)
{
    constexpr auto dash          = QLatin1Char('-');
    constexpr auto equals        = QLatin1Char('=');
    constexpr auto hashTag       = QLatin1Char('#');
    constexpr auto whiteSpace    = QLatin1Char(' ');
    auto           trimmedString = s.trimmed();

    if (trimmedString.isEmpty())
    {
        return 0;
    }
    if (trimmedString.at(indent) == hashTag)
    {
        int heading = count_char(trimmedString, hashTag, indent);
        int strPos  = indent + heading;
        if (strPos < trimmedString.count() && trimmedString[strPos] == whiteSpace)
        {
            int length    = trimmedString.count() - strPos - 1;
            m_header_text = QStringRef(&trimmedString, strPos + 1, length).toString();
            m_position    = m_block_current.begin().fragment().position();
            return heading;
        }
    }
    auto nextBlock = m_block_current.next();
    auto nextLine  = nextBlock.text().trimmed();
    if (nextLine.isEmpty())
    {
        return 0;
    }

    m_header_text = trimmedString;
    m_position    = m_block_current.begin().fragment().position();

    if (contains_only(nextLine, equals) && trimmedString.count() > 0)
    {
        return 1;
    }
    else if (contains_only(nextLine, dash) && trimmedString.count() > 0)
    {
        return 2;
    }
    return 0;
}

HeaderIterator::HeaderIterator(const QTextDocument &document)
{
    m_block_current = document.begin();
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

QString
HeaderIterator::headerText() const
{
    return m_header_text;
}

int
HeaderIterator::position() const
{
    return m_position;
}

void
HeaderIterator::iterate()
{
    for (; m_block_current.isValid();)
    {
        const int indent  = m_block_current.blockFormat().indent();
        int       heading = headerLevel(m_block_current.text(), indent);
        if (heading > 0)
        {
            m_level = heading;
            break;
        }
        m_block_current = m_block_current.next();
    }
}
