#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <algorithm>

#include <QString>

#include <catch.hpp>

int
getIndent(const QString &s)
{
    int spaces = 0;
    for(const auto & c: s)
    {
        if(c != QLatin1Char(' '))
        {
            break;
        }
        spaces++;
    }
    return spaces;
}

QString
indentString(int indent, const QString &s)
{
    QString prefix(indent, QLatin1Char(' '));
    return prefix + s;
}

int
getHeadingLevel(const QString & s)
{
    constexpr auto hashTag = QLatin1Char('#');
    int heading = 0;
    int indent = getIndent(s);
    if(s.at(indent) == hashTag)
    {
        int strPos = indent;

        // TODO use find_if
        for(;strPos < s.count() && s[strPos] == hashTag; strPos++){}

        if(strPos < s.count() && s[strPos] == QLatin1Char(' '))
        {
            heading = strPos - indent;
        }
    }
    return heading;
}

TEST_CASE( "Determines the indent of a QString.", "[getIndent]" ) {
    QString s1{"Hallo"};
    REQUIRE(getIndent(indentString(5, s1)) == 5);
    REQUIRE(getIndent(indentString(1, s1)) == 1);
    REQUIRE(getIndent(s1) == 0);
    REQUIRE(getIndent(QString("")) == 0);
}

TEST_CASE( "Determines the heading level of a markdown text.", "[getHeadingLevel]" ) {
    REQUIRE(getHeadingLevel("# Foo") == 1);
    REQUIRE(getHeadingLevel("## Foo") == 2);
    REQUIRE(getHeadingLevel("### Foo") == 3);
    REQUIRE(getHeadingLevel("#### Foo") == 4);
    REQUIRE(getHeadingLevel("####Foo") == 0);
    REQUIRE(getHeadingLevel("###") == 0);
    REQUIRE(getHeadingLevel("  #### Foo") == 4);
}
