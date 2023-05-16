#ifndef STARTLISTCOMPETITORSTATUS_H
#define STARTLISTCOMPETITORSTATUS_H


class StartListCompetitorStatus
{
public:
    StartListCompetitorStatus();

    enum JumpStatus{
        Finished,
        Unfinished,
        Dsq,
        Dns
    };
    enum AdvanceStatus{
        SureAdvanced,
        SureDroppedOut,
        Waiting
    };

private:
    int jumpStatus;
    int advanceStatus;
    bool qualifiedBy95HSRule;

public:
    int getJumpStatus() const;
    void setJumpStatus(int newJumpStatus);
    int getAdvanceStatus() const;
    void setAdvanceStatus(int newAdvanceStatus);
};

#endif // STARTLISTCOMPETITORSTATUS_H
