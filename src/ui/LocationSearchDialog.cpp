#include "LocationSearchDialog.h"
#include "interfaces/IGeocoder.h"
#include "./ui_locationsearchdialog.h"

LocationSearchDialog::LocationSearchDialog(ApplicationContext *ctx, QWidget *parent) :
    QDialog(parent), ui(new Ui::LocationSearchDialog), m_ctx(ctx) {
    ui->setupUi(this);
    setFixedSize(420, 360);

    connect(ui->searchLocationLineEdit, &QLineEdit::textChanged, this, &LocationSearchDialog::onTextChanged);
    connect(ui->resultListWidget, &QListWidget::itemClicked, this, &LocationSearchDialog::onItemClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

LocationSearchDialog::~LocationSearchDialog() {
    m_selected = {};
    delete ui;
}

void LocationSearchDialog::onTextChanged(const QString &text) {
    if (text.trimmed().size() < 3) return;
    auto geocoder = m_ctx->geocoder();
    const auto results = geocoder->search(text);

    ui->resultListWidget->clear();
    for (const auto &location : results) {
        auto *item = new QListWidgetItem(
            QString("%1 - %2 (%3:%4)").
                arg(location.name).
                arg(location.region).
                arg(location.latitude, 0, 'f', 2).
                arg(location.longitude, 0, 'f', 2)
                );

        item->setData(Qt::UserRole, location.name);
        item->setData(Qt::UserRole + 1, location.region);
        item->setData(Qt::UserRole + 2, location.latitude);
        item->setData(Qt::UserRole + 3, location.longitude);

        ui->resultListWidget->addItem(item);
    }
}

void LocationSearchDialog::onItemClicked(const QListWidgetItem *item) {
    m_selected.name = item->data(Qt::UserRole).toString();
    m_selected.region = item->data(Qt::UserRole + 1).toString();
    m_selected.latitude = item->data(Qt::UserRole + 2).toDouble();
    m_selected.longitude = item->data(Qt::UserRole + 3).toDouble();

    accept();
}
