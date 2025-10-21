#ifndef LOCATIONSEARCHDIALOG_H
#define LOCATIONSEARCHDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTimer>

#include "ApplicationContext.h"
#include "model/Location.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LocationSearchDialog; }
QT_END_NAMESPACE

class ApplicationContext;

class LocationSearchDialog final : public QDialog {
Q_OBJECT

public:
    explicit LocationSearchDialog(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~LocationSearchDialog() override;

    [[nodiscard]]
    Location selected() const { return m_selected; }

private slots:
    void onSearchTextChanged(const QString &text);
    void onItemDoubleClicked();
    void performSearch();
    void onClearClicked();

private:
    Ui::LocationSearchDialog *ui;
    ApplicationContext *m_ctx;
    Location m_selected;
    std::vector<Location> m_results;
    QTimer m_debounceTimer;

    void showResults(const std::vector<Location> &results);
};


#endif //LOCATIONSEARCHDIALOG_H
