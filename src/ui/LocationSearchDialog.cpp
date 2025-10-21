#include "LocationSearchDialog.h"
#include "interfaces/IGeocoder.h"
#include "./ui_locationsearchdialog.h"

LocationSearchDialog::LocationSearchDialog(ApplicationContext *ctx, QWidget *parent) :
    QDialog(parent), ui(new Ui::LocationSearchDialog), m_ctx(ctx) {
    ui->setupUi(this);
    resize(400, 480);
    setWindowTitle(tr("Location Search"));

    // buttons
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->clearButton, &QPushButton::clicked, this, &LocationSearchDialog::onClearClicked);

    // search
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &LocationSearchDialog::onSearchTextChanged);
    connect(ui->resultsList, &QListWidget::itemDoubleClicked, this, &LocationSearchDialog::onItemDoubleClicked);

    // timer configuration
    m_debounceTimer.setSingleShot(true);
    m_debounceTimer.setInterval(300); // 300 ms
    connect(&m_debounceTimer, &QTimer::timeout, this, &LocationSearchDialog::performSearch);
}

LocationSearchDialog::~LocationSearchDialog() {
    m_selected = {};
    delete ui;
}

void LocationSearchDialog::onSearchTextChanged(const QString &text) {
    if (text.trimmed().isEmpty()) {
        ui->resultsList->clear();
        return;
    }

    m_debounceTimer.start();
}

void LocationSearchDialog::onItemDoubleClicked() {
    int row = ui->resultsList->currentRow();
    if (row >= 0 && row < static_cast<int>(m_results.size())) {
        m_selected = m_results[row];
        accept();
    }
}

void LocationSearchDialog::performSearch() {
    const QString q = ui->searchEdit->text().trimmed();
    if (q.isEmpty()) {
        return;
    }

    auto geocoder = m_ctx->geocoder();
    const auto results = geocoder->search(q, QLocale().name().split('_').first(), 10);

    showResults(results);
}

void LocationSearchDialog::onClearClicked() {
    ui->searchEdit->clear();
    ui->resultsList->clear();
}

void LocationSearchDialog::showResults(const std::vector<Location> &results) {
    m_results = results;
    ui->resultsList->clear();

    for (const auto &location : results) {
        auto *item = new QListWidgetItem(ui->resultsList);
        QString line = QString("%1 - %2").arg(location.name, location.region);
        item->setText(line);
        item->setToolTip(QString("%1\nlat=%2, lon=%3").
            arg(line).arg(location.latitude, 0, 'f', 4).
            arg(location.longitude, 0, 'f', 4));

        ui->resultsList->addItem(item);
    }

    if (ui->resultsList->count() == 0) {
        auto *empty = new QListWidgetItem(tr("No results found."));
        empty->setFlags(Qt::NoItemFlags);
        ui->resultsList->addItem(empty);
    }
}
