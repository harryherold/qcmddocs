#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file

#include <algorithm>
#include <functional>

#include <QString>

#include <catch.hpp>

inline QString
indentString(int indent, const QString &s)
{
    QString prefix(indent, QLatin1Char(' '));
    return prefix + s;
}

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

using nextFunctor = std::function<QString(void)>;

int
getHeadingLevel(const QString &s, nextFunctor nextCb)
{
    constexpr auto dash          = QLatin1Char('-');
    constexpr auto equals        = QLatin1Char('=');
    constexpr auto hashTag       = QLatin1Char('#');
    constexpr auto whiteSpace    = QLatin1Char(' ');
    auto           trimmedString = s.trimmed();

    int indent = count_char(trimmedString, whiteSpace);

    if (trimmedString.at(indent) == hashTag)
    {
        int heading = count_char(trimmedString, hashTag, indent);
        int strPos  = indent + heading;

        if (strPos < trimmedString.count() && trimmedString[strPos] == whiteSpace)
        {
            return heading;
        }
    }
    auto nextLine = nextCb().trimmed();
    if (nextLine.isEmpty())
        return 0;

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

TEST_CASE("Determines the indent of a QString.", "[getIndent]")
{
    QString s1{"Hallo"};
    auto    getIndent = [](const QString &s) -> int { return count_char(s, QLatin1Char(' ')); };
    REQUIRE(getIndent(indentString(5, s1)) == 5);
    REQUIRE(getIndent(indentString(1, s1)) == 1);
    REQUIRE(getIndent(s1) == 0);
    REQUIRE(getIndent(QString("")) == 0);
}

TEST_CASE("Determines the heading level of a markdown text.", "[getHeadingLevel]")
{
    auto emptyString = []() -> QString { return QString{""}; };

    REQUIRE(getHeadingLevel("# Foo", emptyString) == 1);
    REQUIRE(getHeadingLevel("## Foo", emptyString) == 2);
    REQUIRE(getHeadingLevel("### Foo", emptyString) == 3);
    REQUIRE(getHeadingLevel("#### Foo", emptyString) == 4);
    REQUIRE(getHeadingLevel("####Foo", emptyString) == 0);
    REQUIRE(getHeadingLevel("###", emptyString) == 0);
    REQUIRE(getHeadingLevel("  #### Foo", emptyString) == 4);
    REQUIRE(getHeadingLevel("# ", emptyString) == 0);

    auto f1 = []() -> QString { return QString{"-------"}; };
    REQUIRE(getHeadingLevel("Foo", f1) == 2);

    auto f2 = []() -> QString { return QString{"======"}; };
    REQUIRE(getHeadingLevel("Foo", f2) == 1);

    auto f3 = []() -> QString { return QString{"==Hallo"}; };
    REQUIRE(getHeadingLevel("Foo", f3) == 0);
}
