#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <QShowEvent>

namespace Ui {
    class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferenceDialog(QWidget *parent = nullptr);
    ~PreferenceDialog();

    void showEvent(QShowEvent *event);
    void changeEvent(QEvent *event);

signals:
    void OkOrApplyClicked();

public slots:
    void updateFromSettings();

    void updateToSettings();

private slots:
    void __updateWindowPropTextEdit(int index);
    void __openSelectLogDirDiag();

private:
    Ui::PreferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_H
