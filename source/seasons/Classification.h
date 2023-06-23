#ifndef ABSTRACTCLASSIFICATION_H
#define ABSTRACTCLASSIFICATION_H


class Classification
{
public:
    Classification();

protected:
    short classificationType;
    short pointsType;
    bool saveStageResults;

    enum PointsType{
        PlacesPoints,
        CompetitionPoints
    };
public:
    short getClassificationType() const;
    void setClassificationType(short newClassificationType);
    short getPointsType() const;
    void setPointsType(short newPointsType);
    bool getSaveStageResults() const;
    void setSaveStageResults(bool newSaveStageResults);
};

#endif // ABSTRACTCLASSIFICATION_H
