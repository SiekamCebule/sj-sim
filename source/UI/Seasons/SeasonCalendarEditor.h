#ifndef SEASONCALENDAREDITOR_H
#define SEASONCALENDAREDITOR_H

#include <QWidget>

namespace Ui {
class SeasonCalendarEditor;
}

class SeasonCalendarEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SeasonCalendarEditor(QWidget *parent = nullptr);
    ~SeasonCalendarEditor();

private:
    Ui::SeasonCalendarEditor *ui;
};

#endif // SEASONCALENDAREDITOR_H
