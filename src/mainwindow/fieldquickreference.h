#ifndef FIELDQUICKREFERENCE_H
#define FIELDQUICKREFERENCE_H

#include <QWidget>
#include <QEvent>
#include <QMap>

namespace Ui {
    class FieldQuickReference;
}

class FieldQuickReference : public QWidget
{
    Q_OBJECT

public:
    explicit FieldQuickReference(QWidget *parent = nullptr);
    ~FieldQuickReference();

    void changeEvent(QEvent *event) override;

    /**
     * @brief showInformation
     * Show information about specified field of specified descriptor
     * @param descriptor
     * @param field
     * @return If this field is exists
     */
    bool showInformation(const QString& descriptor, const QString &field);

public slots:
    void showInformationByUrl(const QUrl &url);

private:
    class Database {
    public:
        Database();
        const QStringList &getFieldNames(const QString &descriptor) const;
        QString getFieldInformation(const QString &descriptor, const QString &field);
        void updateTranslations();

    private:
        QMap<QString, const QStringList&(*)()> _getFieldNames;
        QMap<QString, QString(*)(const QString &)> _getFieldInformation;
    } _db;

    Ui::FieldQuickReference *ui;
};

#endif // FIELDQUICKREFERENCE_H
