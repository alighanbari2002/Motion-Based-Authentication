#include "Pattern.h"

Pattern::Pattern() {}

void Pattern::startNewPattern()
{
    while(pattern.count() > 0)
    {
        pattern.pop_back();
    }
}

void Pattern::addNewSequence(double movement, QString direction, double angle)
{
    QJsonObject qjsobj = {
        {"movement", movement},
        {"direction", direction},
        {"angle", angle}
    };
    pattern.append(qjsobj);
}

bool Pattern::isPatternMatch(Pattern source, Pattern tobeAuthed)
{
    if(source.pattern.size() != tobeAuthed.pattern.size())
    {
        return false;
    }
    for(int i = 0; i < source.pattern.size(); i++)
    {
        double sourcemovement = source.pattern.at(i).toObject().value("movement").toDouble();
        double tobeAuthedmovement = tobeAuthed.pattern.at(i).toObject().value("movement").toDouble();
        double moveLowerBound = fabs(sourcemovement) * (1-moveTolerance);
        double moveHigherBound = fabs(sourcemovement) * (1+moveTolerance);
        double angle = source.pattern.at(i).toObject().value("angle").toDouble();
        QString direction = source.pattern.at(i).toObject().value("direction").toString();
        if(sourcemovement * tobeAuthedmovement < 0)
        {
            return false;
        }
        if(moveLowerBound > fabs(tobeAuthedmovement) ||
            moveHigherBound < fabs(tobeAuthedmovement))
        {
            return false;
        }
        if(angle != tobeAuthed.pattern.at(i).toObject().value("angle").toDouble())
        {
            return false;
        }
        if(direction !=  tobeAuthed.pattern.at(i).toObject().value("direction").toString())
        {
            return false;
        }
    }
    return true;
}

