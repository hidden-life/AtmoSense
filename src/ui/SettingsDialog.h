#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

class SettingsManager;

class SettingsDialog final : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(std::shared_ptr<SettingsManager> settings, QWidget *parent = nullptr);
    ~SettingsDialog() override;

public slots:
    void retranslate();

private:
    Ui::SettingsDialog *ui;
    std::shared_ptr<SettingsManager> m_settings;

private slots:
    void onSaveButtonClicked();
    void onCancelButtonClicked();
};


#endif //SETTINGSDIALOG_H
