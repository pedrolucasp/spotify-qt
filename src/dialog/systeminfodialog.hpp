#pragma once

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
	explicit SystemInfoDialog(QWidget *parent = nullptr);
	static QString systemInfo();
};
