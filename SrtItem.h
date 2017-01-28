#ifndef SRTITEM_H
#define SRTITEM_H

#include <QtCore>

struct SrtItem
{
	int number;
	QTime startTimestamp;
	QTime stopTimestamp;
	QString text;
};

using SrtItemPtr = QSharedPointer<SrtItem>;
using SrtItemPtrList = QList<SrtItemPtr>;

#endif // SRTITEM_H
