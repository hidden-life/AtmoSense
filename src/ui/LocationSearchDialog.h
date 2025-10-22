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

    void populateRecentLocations();
    void performSearch(const QString &searchTxt);

    [[nodiscard]]
    Location selected() const { return m_selected; }

private slots:
    void onSearchTextChanged(const QString &text);
    void onClearClicked();

private:
    Ui::LocationSearchDialog *ui;
    ApplicationContext *m_ctx;
    Location m_selected;
    std::vector<Location> m_results;
    QTimer m_debounceTimer;
};


#endif //LOCATIONSEARCHDIALOG_H
