#include "playback.hpp"

spt::Playback::Playback()
{
}

spt::Playback::Playback(const QJsonObject &json)
{
	if (!json["item"].isNull())
	{
		progressMs	= json["progress_ms"].toInt();
		item 		= Track(json["item"].toObject());
		isPlaying 	= json["is_playing"].toBool();
		volume 		= json["device"].toObject()["volume_percent"].toInt();
		repeat		= json["repeat_state"].toString();
		shuffle		= json["shuffle_state"].toBool();
	}
}

QVariantMap spt::Playback::metadata() const
{
	QString itemName(isPlaying ? item.name : "");
	QStringList itemArtist(isPlaying ? item.artist : "");
	QString itemAlbum(isPlaying ? item.album : "");
	QString itemId(isPlaying ? item.id : "");
	auto itemDuration = isPlaying ? item.duration : 0;
	QString itemImage(isPlaying ? item.image : "");

	auto metadata = QVariantMap(
		{
			{"xesam:title", itemName},
			{"xesam:artist", itemArtist},
			{"xesam:album", itemAlbum},
			{"xesam:albumArtist", itemArtist},
			{"xesam:url", QString("https://open.spotify.com/track/%1").arg(itemId)},
			{"mpris:length", (qulonglong) itemDuration * 1000},
			{"mpris:artUrl", itemImage},
			{"mpris:trackid", QString("spotify:track:%1").arg(itemId)},
		}
	);
	return metadata;
}
QJsonObject spt::Playback::toJson() const
{
	return QJsonObject(
		{
			QPair<QString, int>("progress_ms", progressMs),
			QPair<QString, QJsonObject>("item", item.toJson()),
			QPair<QString, bool>("is_playing", isPlaying),
			QPair<QString, int>("volume", volume),
			QPair<QString, QString>("repeat", repeat),
			QPair<QString, bool>("shuffle", shuffle)
		}
	);
}
