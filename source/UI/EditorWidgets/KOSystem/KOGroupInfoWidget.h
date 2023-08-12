#ifndef KOGROUPINFOWIDGET_H
#define KOGROUPINFOWIDGET_H

#include <QWidget>
#include "../../../competitions/KOSystem/KOGroup.h"

namespace Ui {
class KOGroupListView;
}

class KOGroupInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KOGroupInfoWidget(QWidget *parent = nullptr);
    ~KOGroupInfoWidget();

signals:
    void listViewDoubleClicked();

private:
    Ui::KOGroupListView *ui;
    KOGroup * group;
};

#endif // KOGROUPINFOWIDGET_H
