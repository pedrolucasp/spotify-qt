#pragma once

namespace spt { class Spotify; }

#include "../dialog/deviceselectdialog.hpp"
#include "artist.hpp"
#include "audiofeatures.hpp"
#include "device.hpp"
#include "playback.hpp"
#include "playlist.hpp"
#include "searchresults.hpp"
#include "../settings.hpp"
#include "user.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QSettings>
#include <QString>
#include <QtNetwork>

namespace spt
{
	class Spotify : public QObject
	{
		Q_OBJECT

	public:
		enum class FollowType
		{
			Artist,
			User
		};

		explicit Spotify(Settings &settings, QObject *parent = nullptr);

		QJsonDocument get(const QString &url);
		QVector<Playlist> playlists();
		QVector<Device> devices();
		QString setDevice(const Device &device);
		QString playTracks(const QString &track, const QString &context);
		QString playTracks(const QString &track, const QStringList &all);
		QString playTracks(const QString &context);
		QString setShuffle(bool enabled);
		QString pause();
		QString resume();
		QString seek(int position);
		QString next();
		QString previous();
		QString setVolume(int volume);
		QString setRepeat(const QString &state);
		AudioFeatures trackAudioFeatures(QString trackId);
		QVector<Track> albumTracks(const QString &albumID);
		Artist artist(const QString &artistId);
		Playlist playlist(const QString &playlistId);
		QString addToPlaylist(const QString &playlistId, const QString &trackId);
		QString removeFromPlaylist(const QString &playlistId, const QString &trackId, int pos);
		SearchResults search(const QString &query);
		QVector<Artist> topArtists();
		QVector<Track> topTracks();
		QVector<Album> savedAlbums();
		QVector<Track> savedTracks(int offset = 0);
		QString addSavedTrack(const QString &trackId);
		QString removeSavedTrack(const QString &trackId);
		QVector<Track> recentlyPlayed();
		QVector<Album> newReleases(int offset = 0);
		User me();
		QString editPlaylist(const Playlist &playlist);
		QString addToQueue(const QString &uri);
		QVector<Artist> followedArtists(const QString &offset = QString());
		QVector<bool> isFollowing(FollowType type, const QStringList &ids);
		void follow(FollowType type, const QStringList &ids);
		void unfollow(FollowType type, const QStringList &ids);

	public slots:
		void requestCurrentPlayback();

	private slots:
		void getLater(const QString &url);

	signals:
		void got(const QJsonDocument &json);
		void gotPlayback(const Playback &playback);

	private:
		qint64		lastAuth;
		QString		currentDevice;
		Settings	&settings;
		QNetworkAccessManager *networkManager;
		/**
		 * Prepare network request with auth header
		 */
		QNetworkRequest request(const QString &url);
		/**
		 * HTTP PUT request expecting JSON response
		 */
		QString put(const QString &url, QVariantMap *body = nullptr);
		QString post(const QString &url);
		QString del(const QString &url, QVariantMap *body);
		static QString errorMessage(QNetworkReply *reply);

		template <class T>
		QVector<T> loadItems(const QString &url);

		QVector<Track> albumTracks(const QString &albumId, const QString &albumName, int offset);

		bool refresh();
		static QString followTypeString(FollowType type);
	};
}