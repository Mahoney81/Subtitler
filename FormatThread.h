#ifndef FORMATTHREAD_H
#define FORMATTHREAD_H

#include <QtCore>
#include "SrtItem.h"

class FormatThread : public QThread
{
	Q_OBJECT
public:
	FormatThread(const QString& filePath, const SrtItemPtrList& dataList, bool isShowTimeStamps, QObject *pParent);
	virtual ~FormatThread(void);

	virtual void run(void) override;

signals:
	void progressChanged(const int percent);
	void htmlReady(const QString& filePath, const SrtItemPtrList& dataList, const QString& html);

private:
	const QString m_filePath;
	const SrtItemPtrList m_dataList;
	const bool m_isShowTimeStamps;
	bool m_isRunning = false;
};

#endif // FORMATTHREAD_H
