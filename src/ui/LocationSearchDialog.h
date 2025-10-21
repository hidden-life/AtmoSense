#ifndef LOCATIONSEARCHDIALOG_H
#define LOCATIONSEARCHDIALOG_H

#include <QDialog>
#include <QListWidget>

#include "ApplicationContext.h"
#include "model/Location.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LocationSearchDialog; }
QT_END_NAMESPACE

class LocationSearchDialog final : public QDialog {
Q_OBJECT

public:
    explicit LocationSearchDialog(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~LocationSearchDialog() override;

    Location selected() const { return m_selected; }

private slots:
    void onTextChanged(const QString &text);
    void onItemClicked(const QListWidgetItem *item);

private:
    Ui::LocationSearchDialog *ui;
    ApplicationContext *m_ctx;
    Location m_selected;
};


#endif //LOCATIONSEARCHDIALOG_H
