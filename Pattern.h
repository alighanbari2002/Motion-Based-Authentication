#ifndef PATTERN_H
#define PATTERN_H

#include <QJsonObject>
#include <QJsonArray>

const double moveTolerance = 0.8;
const double angleTolerance = 0.25;


class Pattern
{
private:
    QJsonArray pattern;



public:
    Pattern();


    void startNewPattern();
    void addNewSequence(double movement, QString direction, double angle);
    static bool isPatternMatch(Pattern source, Pattern tobeAuthed);

};

#endif // PATTERN_H
