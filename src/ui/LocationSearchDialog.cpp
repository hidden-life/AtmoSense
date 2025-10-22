#include "LocationSearchDialog.h"
#include "interfaces/IGeocoder.h"
#include "./ui_locationsearchdialog.h"
#include "Logger.h"
#include "SettingsManager.h"

namespace {
    constexpr int DEBOUNCE_MS = 300;
}

LocationSearchDialog::LocationSearchDialog(ApplicationContext *ctx, QWidget *parent) :
    QDialog(parent), ui(new Ui::LocationSearchDialog), m_ctx(ctx) {
    ui->setupUi(this);
    resize(400, 480);
    setWindowTitle(tr("Location Search"));

    ui->resultsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->resultsList->setWordWrap(true);

    // buttons
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->clearButton, &QPushButton::clicked, this, &LocationSearchDialog::onClearClicked);

    // search
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &LocationSearchDialog::onSearchTextChanged);

    // timer configuration
    m_debounceTimer.setSingleShot(true);
    m_debounceTimer.setInterval(DEBOUNCE_MS); // 300 ms
    connect(&m_debounceTimer, &QTimer::timeout, this, [this] {
        performSearch(ui->searchEdit->text().trimmed());
    });

    connect(ui->resultsList, &QListWidget::itemDoubleClicked, this, [this](const QListWidgetItem *item) {
        if (const auto idx = ui->resultsList->row(item); idx >= 0 && idx < static_cast<int>(m_results.size())) {
            m_selected = m_results[idx];
            accept();
        }
    });

    populateRecentLocations();
}

LocationSearchDialog::~LocationSearchDialog() {
    m_selected = {};
    delete ui;
}

void LocationSearchDialog::populateRecentLocations() {
    ui->resultsList->clear();
    m_results.clear();

    const auto recents = m_ctx->settings()->recentLocations();
    if (recents.empty()) return;

    for (const auto &loc : recents) {
        m_results.push_back(loc);
        auto *it = new QListWidgetItem();
        it->setText(loc.name + " - " + loc.country + (loc.region.isEmpty() ? "" : (", " + loc.region)));
        it->setToolTip(QString("%1\nlat=%2, lon=%3").arg(loc.name).arg(loc.latitude).arg(loc.longitude));
        ui->resultsList->addItem(it);
    }
}

void LocationSearchDialog::performSearch(const QString &searchTxt) {
    ui->resultsList->clear();
    m_results.clear();

    if (searchTxt.isEmpty()) {
        populateRecentLocations();
        return;
    }

    try {
        const QString lang = LanguageUtils::toCode(m_ctx->settings()->language());
        const auto results = m_ctx->currentGeocoderProvider()->search(searchTxt, lang, 20);
        for (const auto &loc : results) {
            m_results.push_back(loc);

            auto *it = new QListWidgetItem();
            QString line = loc.name;
            if (!loc.country.isEmpty()) line += " - " + loc.country;
            if (!loc.region.isEmpty()) line += ", " + loc.region;

            it->setText(line);
            it->setToolTip(QString("%1\nlat=%2, lon=%3").arg(loc.name).arg(loc.latitude).arg(loc.longitude));
            ui->resultsList->addItem(it);
        }

        if (m_results.empty()) {
            auto *it = new QListWidgetItem(tr("No results."));
            it->setFlags(Qt::NoItemFlags);
            ui->resultsList->addItem(it);
        }
    } catch (const std::exception &e) {
        Logger::error(QString("Geocoder failed: %1").arg(e.what()));
        auto *it = new QListWidgetItem(tr("Search error..."));
        it->setFlags(Qt::NoItemFlags);
        ui->resultsList->addItem(it);
    }
}

void LocationSearchDialog::onSearchTextChanged(const QString &text) {
    if (text.trimmed().isEmpty()) {
        ui->resultsList->clear();
        return;
    }

    m_debounceTimer.start();
}

void LocationSearchDialog::onClearClicked() {
    ui->searchEdit->clear();
    ui->resultsList->clear();
}