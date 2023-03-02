#ifndef JUMPDATADETAILEDINFOWINDOW_H
#define JUMPDATADETAILEDINFOWINDOW_H

#include <QWidget>
#include "../../simulator/JumpData.h"

namespace Ui {
class JumpDataDetailedInfoWindow;
}

class JumpDataDetailedInfoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit JumpDataDetailedInfoWindow(JumpData * jumpData = nullptr, QWidget *parent = nullptr);
    ~JumpDataDetailedInfoWindow();

    void fillJumpInformations();

    JumpData *getJumpData() const;
    void setJumpData(JumpData *newJumpData);

private:
    Ui::JumpDataDetailedInfoWindow *ui;

    JumpData * jumpData;
};

#endif // JUMPDATADETAILEDINFOWINDOW_H
