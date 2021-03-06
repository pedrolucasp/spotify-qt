#pragma once

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

namespace spt
{
	class Album
	{
	public:
		Album() = default;
		explicit Album(const QJsonObject &json);
		QString id, albumGroup, image, name, artist;
		QDateTime releaseDate;
	};
}