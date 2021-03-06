#include "settingsdialog.hpp"

SettingsDialog::SettingsDialog(Settings &settings, QWidget *parent) : settings(settings), QDialog(parent)
{
	// Main layout
	auto mainLayout = new QVBoxLayout();
	auto tabs = new QTabWidget();
	tabs->addTab(appSettings(), "Application");
	tabs->addTab(interfaceSettings(), "Interface");
	tabs->addTab(traySettings(), "Tray icon");
	tabs->addTab(spotifySettings(), "Spotify");
	tabs->addTab(aboutSettings(), "About");
	mainLayout->addWidget(tabs, 1);
	// Buttons
	auto buttons = new QDialogButtonBox();
	auto okButton = buttons->addButton(QDialogButtonBox::Ok);
	QPushButton::connect(okButton, &QPushButton::clicked, [this](bool checked) {
		if (applySettings())
			accept();
	});
	auto applyButton = buttons->addButton(QDialogButtonBox::Apply);
	QPushButton::connect(applyButton, &QPushButton::clicked, [this](bool checked) {
		applySettings();
	});
	auto cancelButton = buttons->addButton(QDialogButtonBox::Cancel);
	QPushButton::connect(cancelButton, &QPushButton::clicked, [this](bool checked) {
		accept();
	});
	mainLayout->addWidget(buttons);
	// Set layout
	setWindowTitle("Settings");
	setLayout(mainLayout);
	resize(360, 260);
}

QWidget *SettingsDialog::appSettings()
{
	auto layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);
	// Refresh interval
	auto appRefreshLayout = new QHBoxLayout();
	auto appRefreshLabel = new QLabel("Refresh interval", this);
	appRefreshLabel->setToolTip("How often to refresh playback status from the Spotify servers");
	appRefreshLayout->addWidget(appRefreshLabel);
	appRefresh = new QComboBox(this);
	appRefresh->addItems({
		"1", "3", "10"
	});
	appRefresh->setEditable(true);
	appRefresh->setCurrentIndex(-1);
	appRefresh->setEditText(QString::number(settings.general.refreshInterval));
	appRefresh->setValidator(new QIntValidator(1, 60, this));
	appRefreshLayout->addWidget(appRefresh);
	appRefreshLayout->addWidget(new QLabel("seconds"));
	layout->addLayout(appRefreshLayout);
	// PulseAudio volume control
	if (isPulse())
	{
		appPulse = new QCheckBox("PulseAudio volume control", this);
		appPulse->setToolTip(
			"Use PulseAudio for volume control instead, only works if listening on same device");
		appPulse->setChecked(settings.general.pulseVolume);
		layout->addWidget(appPulse);
	}
	// MPRIS D-Bus
#ifdef Q_OS_LINUX
	appMedia = new QCheckBox("Media controller", this);
	appMedia->setChecked(settings.general.mediaController);
	appMedia->setToolTip("Enable media controller through the MPRIS D-Bus interface");
	layout->addWidget(appMedia);
#endif
	// Spotify playback order
	appSptOrder = new QCheckBox("Spotify playback order", this);
	appSptOrder->setToolTip("Use Spotify playback order instead of app list order");
	appSptOrder->setChecked(settings.general.spotifyPlaybackOrder);
	layout->addWidget(appSptOrder);
	// What's new dialog
	appWhatsNew = new QCheckBox("Show what's new on start", this);
	appWhatsNew->setToolTip("Show what's new in the latest version after the app has been updated");
	appWhatsNew->setChecked(settings.general.showChangelog);
	layout->addWidget(appWhatsNew);
	// Final layout
	auto widget = new QWidget();
	widget->setLayout(layout);
	return widget;
}

QWidget *SettingsDialog::interfaceSettings()
{
	auto layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);

	// Dark theme
	itfDark = new QCheckBox("Dark theme", this);
	itfDark->setToolTip("Use custom dark theme");
	itfDark->setChecked(settings.darkTheme());
	layout->addWidget(itfDark);

	// Song header resize mode
	itfResizeAuto = new QCheckBox("Auto resize track list headers", this);
	itfResizeAuto->setToolTip("Automatically resize track list headers to fit content");
	itfResizeAuto->setChecked(settings.general.songHeaderResizeMode == QHeaderView::ResizeToContents);
	layout->addWidget(itfResizeAuto);

	// Always use fallback icons (if system icons are an option)
	if (hasIconTheme())
	{
		itfIcFallback = new QCheckBox("Always use fallback icons", this);
		itfIcFallback->setToolTip("Always use bundled fallback icons, even if system icons are available");
		itfIcFallback->setChecked(settings.general.fallbackIcons);
		layout->addWidget(itfIcFallback);
	}

	// Monospace remaining time
	itfMonoTime = new QCheckBox("Fixed width remaining time", this);
	itfMonoTime->setToolTip("Use a fixed width for remaining time to avoid resizing");
	itfMonoTime->setChecked(settings.general.fixedWidthTime);
	layout->addWidget(itfMonoTime);

	// Context info
	itfContextInfo = new QCheckBox("Show context information", this);
	itfContextInfo->setToolTip("Show what context music is currently playing from above current track");
	itfContextInfo->setChecked(settings.general.showContextInfo);
	layout->addWidget(itfContextInfo);

	// Final layout
	auto widget = new QWidget();
	widget->setLayout(layout);
	return widget;
}

QWidget *SettingsDialog::traySettings()
{
	// Main container for everything
	auto layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);
	// Tray icon settings
	itfTrayIcon = new QCheckBox("Enabled", this);
	itfTrayIcon->setToolTip("Add an icon to the system tray for quick access");
	itfTrayIcon->setChecked(settings.general.trayIcon);
	layout->addWidget(itfTrayIcon);
	// Desktop notifications
	itfTrayNotify = new QCheckBox("Desktop notifications", this);
	itfTrayNotify->setToolTip("Replace status bar with desktop notifications (suppresses any non-error messages)");
	itfTrayNotify->setChecked(settings.general.trayNotifications);
	layout->addWidget(itfTrayNotify);
	QCheckBox::connect(itfTrayNotify, &QCheckBox::stateChanged, [this](int state) {
		if (state == Qt::CheckState::Checked && itfTrayIcon != nullptr) {
			itfTrayIcon->setChecked(true);
		}
	});
	// Invert tray icon
	itfTrayInvert = new QCheckBox("Invert icon", this);
	itfTrayInvert->setToolTip("Invert colors in tray icon to be visible on light backgrounds");
	itfTrayInvert->setChecked(settings.general.trayLightIcon);
	layout->addWidget(itfTrayInvert);
	// Album art in tray
	itfTrayAlbum = new QCheckBox("Album art as icon", this);
	itfTrayAlbum->setToolTip("Show album art of current track in tray icon");
	itfTrayAlbum->setChecked(settings.general.trayAlbumArt);
	layout->addWidget(itfTrayAlbum);
	// Final layout
	auto widget = new QWidget();
	widget->setLayout(layout);
	return widget;
}

QWidget *SettingsDialog::spotifySettings()
{
	// Main container for everything
	auto layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);

	// Executable settings
	auto sptPathLayout = new QHBoxLayout();
	sptPath = new QLineEdit(settings.spotify.path, this);
	sptPath->setPlaceholderText("spotifyd path");
	sptPathLayout->addWidget(sptPath, 1);
	auto sptPathBrowse = new QPushButton("...", this);
	sptPathBrowse->setMaximumWidth(40);
	sptPathBrowse->setFlat(true);
	QAbstractButton::connect(sptPathBrowse, &QAbstractButton::clicked, [this](bool checked) {
		auto filePath = QFileDialog::getOpenFileName(this, "spotifyd path", "/");
		if (!filePath.isEmpty() && sptPath != nullptr)
			sptPath->setText(filePath);
	});
	sptPathLayout->addWidget(sptPathBrowse);
	auto sptOpenLog = new QPushButton(this);
	sptOpenLog->setIcon(Icon::get("folder-txt"));
	sptOpenLog->setToolTip("Open log");
	sptOpenLog->setFlat(true);
	QAbstractButton::connect(sptOpenLog, &QAbstractButton::clicked, [this](bool checked)
	{
		(new ClientHandlerLogDialog(this))->exec();
	});
	sptPathLayout->addWidget(sptOpenLog);
	layout->addLayout(sptPathLayout);

	// Spotifyd version
	sptVersion = new QLabel("(no spotifyd provided)", this);
	if (!settings.spotify.path.isEmpty())
	{
		auto client = spt::ClientHandler::version(settings.spotify.path);
		if (sptVersion != nullptr)
			sptVersion->setText(client);
	}
	sptVersion->setEnabled(false);
	layout->addWidget(sptVersion);

	// Global config
	sptGlobal = new QCheckBox("Use global config", this);
	sptGlobal->setToolTip("Use spotifyd.conf file in either ~/.config/spotifyd or /etc/spotifyd only");
	sptGlobal->setChecked(settings.spotify.globalConfig);
	QCheckBox::connect(sptGlobal, &QCheckBox::stateChanged, this, &SettingsDialog::globalConfigToggle);
	layout->addWidget(sptGlobal);

	// Box and layout for all app specific settings
	sptGroup = new QGroupBox("App specific settings", this);
	sptGroup->setEnabled(!sptGlobal->isChecked());
	auto sptLayout = new QGridLayout();

	// Username
	sptLayout->addWidget(new QLabel("Username", sptGroup), 0, 0);
	sptUsername = new QLineEdit(settings.spotify.username, sptGroup);
	sptLayout->addWidget(sptUsername, 0, 1);

	// Bitrate
	sptLayout->addWidget(new QLabel("Quality", sptGroup), 1, 0);
	sptBitrate = new QComboBox(sptGroup);
	sptBitrate->addItems(
		{
			"Low (96 kbit/s)", "Medium (160 kbit/s)", "High (320 kbit/s)"
		}
	);
	auto bitrate = settings.spotify.bitrate;
	sptBitrate->setCurrentIndex(bitrate == 96 ? 0 : bitrate == 160 ? 1 : 2);
	sptLayout->addWidget(sptBitrate, 1, 1);
	sptGroup->setLayout(sptLayout);
	layout->addWidget(sptGroup);

	// Start with app
	sptAppStart = new QCheckBox("Start with app", this);
	sptAppStart->setToolTip("Start spotifyd together with the app (always starts and doesn't automatically close)");
	sptAppStart->setChecked(settings.spotify.startClient);
	layout->addWidget(sptAppStart);

	// Always start
	sptAlways = new QCheckBox("Always start", this);
	sptAlways->setToolTip("Always start client, even if there are other devices already available");
	sptAlways->setChecked(settings.spotify.alwaysStart);
	layout->addWidget(sptAlways);

	// Final layout
	auto widget = new QWidget();
	widget->setLayout(layout);
	return widget;
}

void SettingsDialog::globalConfigToggle(int state)
{
	sptGroup->setEnabled(state == Qt::Unchecked);
}

QWidget *SettingsDialog::aboutSettings()
{
	auto cacheSize = 0u;
	auto mainWindow = dynamic_cast<MainWindow*>(parentWidget());
	if (mainWindow != nullptr)
		for (auto const &file : QDir(mainWindow->getCacheLocation()).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
			for (auto const &f : QDir(file.absoluteFilePath()).entryInfoList(QDir::Files))
				cacheSize += f.size();

	auto layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);

	// Title
	auto title = new QHBoxLayout();
	title->setAlignment(Qt::AlignHCenter);
	auto titleLogo = new QLabel();
	titleLogo->setPixmap(Icon::get("logo:spotify-qt").pixmap(64, 64));
	title->addWidget(titleLogo);
	layout->addLayout(title);
	auto titleVersion = new QVBoxLayout();
	titleVersion->setSpacing(0);
	titleVersion->setAlignment(Qt::AlignVCenter);
	auto titleAppName = new QLabel("spotify-qt");
	auto appNameFont = titleAppName->font();
	appNameFont.setPointSize(appNameFont.pointSizeF() * 1.5);
	titleAppName->setFont(appNameFont);
	titleVersion->addWidget(titleAppName);
	titleVersion->addWidget(new QLabel(APP_VERSION));
	title->addLayout(titleVersion);

	// User info
	if (mainWindow != nullptr)
		layout->addWidget(new QLabel(QString("Hello %1!")
			.arg(mainWindow->getCurrentUser().displayName)), 0, Qt::AlignHCenter);

	// Grid with buttons
	layout->addSpacing(8);
	auto options = new QGridLayout();

	// About Qt
	auto aboutQt = new QPushButton(Icon::get("logo:qt"),
		QString("About Qt %1.%2")
			.arg(QT_VERSION_MAJOR).arg(QT_VERSION_MINOR));
	aboutQt->setFlat(true);
	QAbstractButton::connect(aboutQt, &QPushButton::clicked, [this](bool checked) {
		QMessageBox::aboutQt(this);
	});
	options->addWidget(aboutQt);

	// Generate report
	auto generateReport = new QPushButton(Icon::get("description"), "System info", this);
	generateReport->setFlat(true);
	QAbstractButton::connect(generateReport, &QPushButton::clicked, [this, mainWindow](bool checked) {
		(new SystemInfoDialog(mainWindow, this))->show();
	});
	options->addWidget(generateReport, 0, 1);

	// Open cache directory
	auto openCache = new QPushButton(Icon::get("folder-temp"),
		QString("Open cache directory (%1M)").arg(cacheSize / 1000 / 1000));
	openCache->setFlat(true);
	QAbstractButton::connect(openCache, &QPushButton::clicked, [this, mainWindow](bool checked) {
		if (mainWindow == nullptr)
			return;
		Utils::openUrl(mainWindow->getCacheLocation(), LinkType::Web, this);
	});
	options->addWidget(openCache);

	// Open config file
	auto openConfig = new QPushButton(Icon::get("folder-txt"), "Open config file");
	openConfig->setFlat(true);
	QAbstractButton::connect(openConfig, &QPushButton::clicked, [this](bool checked) {
		Utils::openUrl(Settings::fileName(), LinkType::Path, this);
	});
	options->addWidget(openConfig);
	layout->addLayout(options, 1);

	// Final layout
	return Utils::layoutToWidget(layout);
}

bool SettingsDialog::applySettings()
{
	// Set theme
	auto changeTheme = itfDark->isChecked() != settings.darkTheme();
	if (changeTheme)
	{
		QMessageBox::information(
			this, "Dark Theme",
			"Please restart the application to fully apply selected theme");
		settings.setDarkTheme(itfDark->isChecked());
		QApplication::setStyle(settings.general.style);
		Utils::applyPalette(settings.general.stylePalette);
	}

	// Media controller
	if (appMedia != nullptr)
	{
		if (appMedia->isChecked() != settings.general.mediaController)
			QMessageBox::information(
				this, "Media Controller",
				"Please restart the application to apply changes");
		settings.general.mediaController = appMedia->isChecked();
	}

	// PulseAudio volume
	if (appPulse != nullptr)
		settings.general.pulseVolume = appPulse->isChecked();

	// Check spotify client path
	if (!sptPath->text().isEmpty())
	{
		auto client = spt::ClientHandler::version(sptPath->text());
		if (client.isEmpty())
		{
			applyFail("spotifyd path");
			return false;
		}
		sptVersion->setText(client);
		settings.spotify.path = sptPath->text();
	}

	// Desktop notifications and tray icon
	if (itfTrayNotify->isChecked() && !itfTrayIcon->isChecked())
	{
		itfTrayIcon->setChecked(true);
		QMessageBox::information(
			this, "Desktop Notifications",
			"Desktop notifications requires tray icon to be enabled, so it was enabled");
	}
	// Check if tray icon needs to be reloaded
	auto reloadTray = settings.general.trayIcon != itfTrayIcon->isChecked()
		|| settings.general.trayNotifications != itfTrayNotify->isChecked()
		|| settings.general.trayLightIcon != itfTrayInvert->isChecked();
	// Apply
	settings.general.trayIcon = itfTrayIcon->isChecked();
	settings.general.trayNotifications = itfTrayNotify->isChecked();
	settings.general.trayLightIcon = itfTrayInvert->isChecked();
	settings.general.trayAlbumArt = itfTrayAlbum->isChecked();
	// Reload if needed
	auto window = dynamic_cast<MainWindow*>(parent());
	if (reloadTray && window != nullptr)
		window->reloadTrayIcon();
	// Song header resize mode
	auto resizeMode = itfResizeAuto->isChecked()
		? QHeaderView::ResizeToContents
		: QHeaderView::Interactive;
	if (resizeMode != settings.general.songHeaderResizeMode && window != nullptr)
		window->getSongsTree()->header()->setSectionResizeMode(resizeMode);
	settings.general.songHeaderResizeMode = resizeMode;

	// Refresh interval
	auto ok = false;
	auto interval = appRefresh->currentText().toInt(&ok);
	if (!ok || interval <= 0 || interval > 60)
	{
		applyFail("refresh interval");
		return false;
	}
	settings.general.refreshInterval = interval;

	// Spotify global config
	if (sptGlobal->isChecked() && !sptConfigExists())
		QMessageBox::warning(this,
			"spotifyd config not found",
			QString("Couldn't find a config file for spotifyd. You may experience issues."));
	settings.spotify.globalConfig = sptGlobal->isChecked();

	// Other application stuff
	settings.general.showChangelog = appWhatsNew->isChecked();
	settings.general.spotifyPlaybackOrder = appSptOrder->isChecked();

	// Other interface stuff
	if (itfIcFallback != nullptr)
		settings.general.fallbackIcons = itfIcFallback->isChecked();
	if (window != nullptr)
		window->setFixedWidthTime(itfMonoTime->isChecked());
	settings.general.fixedWidthTime = itfMonoTime->isChecked();
	settings.general.showContextInfo = itfContextInfo->isChecked();

	// Other Spotify stuff
	settings.spotify.startClient =sptAppStart->isChecked();
	settings.spotify.username = sptUsername->text();
	auto bitrate = sptBitrate->currentIndex();
	settings.spotify.bitrate = bitrate == 0 ? 96 : bitrate == 1 ? 160 : 320;
	settings.spotify.alwaysStart = sptAlways->isChecked();

	// Everything is fine
	settings.save();
	return true;
}

void SettingsDialog::applyFail(const QString &setting)
{
	QMessageBox::warning(this,
		"Failed to apply settings",
		QString("Failed to apply setting \"%1\". Check your settings and try again.").arg(setting));
}

bool SettingsDialog::isPulse()
{
	// Assume /usr/bin/pactl
	return QFileInfo("/usr/bin/pactl").isExecutable();
}

bool SettingsDialog::sptConfigExists()
{
	// Config is either ~/.config/spotifyd/spotifyd.conf or /etc/spotifyd/spotifyd.conf
	return QFile(QString("%1/.config/spotifyd/spotifyd.conf")
		.arg(QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0])).exists()
		|| QFile("/etc/spotifyd/spotifyd.conf").exists();
}

bool SettingsDialog::hasIconTheme()
{
	return !QIcon::fromTheme("media-playback-start").isNull();
}