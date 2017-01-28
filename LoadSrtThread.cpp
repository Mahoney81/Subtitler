#include "stdafx.h"
#include "LoadSrtThread.h"

LoadSrtThread::LoadSrtThread(const QString& filePath, QObject *pParent) : QThread(pParent), m_filePath(filePath)
{
}

LoadSrtThread::~LoadSrtThread(void)
{
	requestInterruption();
	while(m_isRunning)
		QThread::wait(1);
}

void LoadSrtThread::run(void)
{
	m_isRunning = true;

	SrtItemPtrList dataList;
	const QString fileName = QFileInfo(m_filePath).fileName();
	enum class Stage { Number, TimeFrame, Text };
	Stage stage = Stage::Number;

	QFile f(m_filePath);

	try
	{
		if(f.open(QIODevice::ReadOnly))
		{
			const qint64 fileSize = f.size();
			SrtItemPtr pItem;
			while(!f.atEnd())
			{
				if(isInterruptionRequested())
				{
					m_isRunning = false;
					return;
				}
				const QString textLine = f.readLine().simplified();
				if(textLine.isEmpty())
				{
					stage = Stage::Number;
					continue;
				}

				switch (stage)
				{
				case Stage::Number:
					if(pItem)
					{
						pItem->text.chop(1);	// delete last space
						if(!pItem->text.isEmpty())
							dataList << pItem;
					}
					pItem.reset(new SrtItem);
					pItem->number = parseNumber(textLine);
					stage = Stage::TimeFrame;
					break;
				case Stage::TimeFrame:
					if(!pItem)
						throw m_unexpectedFormatError;
					pItem->startTimestamp = parseStartTime(textLine);
					pItem->stopTimestamp = parseStopTime(textLine);
					stage = Stage::Text;
					break;
				case Stage::Text:
					if(!pItem)
						throw m_unexpectedFormatError;
					pItem->text += textLine.simplified() + " ";
					break;
				}

				double percent = f.pos() / double(fileSize) * 100;
				emit progressChanged(percent);
			}

			emit dataLoaded(m_filePath, dataList);
		}
		else
		{
			const QString fileErrorText = f.errorString();
			throw tr("Failed to open '%1' for reading. %2").arg(fileName).arg(fileErrorText).simplified();
		}
	}
	catch(const QString& errorText)
	{
		emit failed(errorText);
	}

	m_isRunning = false;
}

int LoadSrtThread::parseNumber(const QString& text) const
{
	bool isOk = true;
	int number = text.simplified().toInt(&isOk);
	if(!isOk)
		throw m_unexpectedFormatError;
	return number;
}

QTime LoadSrtThread::parseStartTime(const QString& text) const
{
	QTime startTimestamp = QTime::fromString(text.left(12), "HH:mm:ss,zzz");
	if(startTimestamp.isValid())
		return startTimestamp;
	else
		throw m_unexpectedFormatError;
}

QTime LoadSrtThread::parseStopTime(const QString& text) const
{
	QTime stopTimestamp = QTime::fromString(text.right(12), "HH:mm:ss,zzz");
	if(stopTimestamp.isValid())
		return stopTimestamp;
	else
		throw m_unexpectedFormatError;
}
