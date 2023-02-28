#ifndef SINGLEJUMPSRESULTSWINDOW_H
#define SINGLEJUMPSRESULTSWINDOW_H

#include <QDialog>
#include "../../single-jumps/SingleJumpsManager.h"

namespace Ui {
class SingleJumpsResultsWindow;
}

class SingleJumpsResultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SingleJumpsResultsWindow(QWidget *parent = nullptr);
    ~SingleJumpsResultsWindow();

    void fillMiniJumpsResultsLayout();

    SingleJumpsManager *getManager() const;
    void setManager(SingleJumpsManager *newManager);

private:
    Ui::SingleJumpsResultsWindow *ui;

    SingleJumpsManager * manager;
};

#endif // SINGLEJUMPSRESULTSWINDOW_H
