#ifndef JUMPERSFORMGENERATORCONFIGWINDOW_H
#define JUMPERSFORMGENERATORCONFIGWINDOW_H

#include <QDialog>
#include "JumperFormGeneratorsSettingsTableModel.h"

namespace Ui {
class JumpersFormGeneratorConfigWindow;
}

class JumpersFormGeneratorConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit JumpersFormGeneratorConfigWindow(QWidget *parent = nullptr);
    ~JumpersFormGeneratorConfigWindow();

    void updateTable();

private:
    Ui::JumpersFormGeneratorConfigWindow *ui;

    JumperFormGeneratorsSettingsTableModel * tableModel;
public:
    JumperFormGeneratorsSettingsTableModel *getTableModel() const;
public slots:
    void on_doubleSpinBox_tendenceVariability_editingFinished();
    void on_doubleSpinBox_tendBonus_editingFinished();
    void on_doubleSpinBox_tendAlignment_editingFinished();
    void on_doubleSpinBox_minTend_editingFinished();
    void on_doubleSpinBox_maxTend_editingFinished();
    void on_doubleSpinBox_tendVariability_editingFinished();
    void on_doubleSpinBox_formBonus_editingFinished();
    void on_doubleSpinBox_minForm_editingFinished();
    void on_doubleSpinBox_maxForm_editingFinished();
private slots:
    void on_listWidget_questions_doubleClicked(const QModelIndex &index);
};

#endif // JUMPERSFORMGENERATORCONFIGWINDOW_H
