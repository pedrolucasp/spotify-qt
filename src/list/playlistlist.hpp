#pragma once

#include "../mainwindow.hpp"
#include "../spotify/spotify.hpp"
#include "playlistlist.hpp"

#include <QListWidget>

class PlaylistList: public QListWidget
{
	Q_OBJECT

public:
	PlaylistList(spt::Spotify &spotify, QWidget *parent);

private:
	spt::Spotify &spotify;
	QWidget *parent;

	void clicked(QListWidgetItem *item);
	void doubleClicked(QListWidgetItem *item);
	void menu(const QPoint &pos);
};