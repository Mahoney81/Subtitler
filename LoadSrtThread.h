#ifndef LOADSRTTHREAD_H
#define LOADSRTTHREAD_H

#include <QtCore>
#include "SrtItem.h"

class LoadSrtThread : public QThread
{
	Q_OBJECT
public:
	LoadSrtThread(const QString& filePath, QObject *pParent);
	virtual ~LoadSrtThread(void);

	virtual void run(void) override;

signals:
	void progressChanged(const int percent);
	void dataLoaded(const QString& filePath, const SrtItemPtrList& dataList);
	void failed(const QString& reasonWhy);

private:
	const QString m_filePath;
	const QString m_unexpectedFormatError = tr("Unexpected file format");
	bool m_isRunning = false;

	inline int parseNumber(const QString& text) const;
	inline QTime parseStartTime(const QString& text) const;
	inline QTime parseStopTime(const QString& text) const;
};

#endif // LOADSRTTHREAD_H
