#pragma once

#include "../spotify/spotify.hpp"
#include "../mainwindow.hpp"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class SystemInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SystemInfoDialog(QWidget *mainWindow, QWidget *parent = nullptr);

private:
	QString systemInfo();

	QWidget *mainWindow = nullptr;
};
