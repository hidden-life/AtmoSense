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
            QString("%1 (%2, %3) - %4, %5").
                arg(location.name).
                arg(location.latitude, 0, 'f', 2).
                arg(location.longitude, 0, 'f', 2).
                arg(location.country).
                arg(location.region)
                );

        item->setData(Qt::UserRole, location.latitude);
        item->setData(Qt::UserRole + 1, location.longitude);
        item->setData(Qt::UserRole + 2, location.timezone);
        item->setData(Qt::UserRole + 3, location.name);

        ui->resultListWidget->addItem(item);
    }
}

void LocationSearchDialog::onItemClicked(const QListWidgetItem *item) {
    m_selected.name = item->data(Qt::UserRole + 3).toString();
    m_selected.timezone = item->data(Qt::UserRole + 2).toString();
    m_selected.longitude = item->data(Qt::UserRole + 1).toDouble();
    m_selected.latitude = item->data(Qt::UserRole).toDouble();
    m_selected.country = item->data(Qt::UserRole + 4).toString();
    m_selected.region = item->data(Qt::UserRole + 5).toString();

    accept();
}
