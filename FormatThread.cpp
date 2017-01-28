#include "stdafx.h"
#include "FormatThread.h"

FormatThread::FormatThread(const QString& filePath, const SrtItemPtrList& dataList, bool isShowTimeStamps, QObject* pParent) : QThread(pParent), m_filePath(filePath), m_dataList(dataList), m_isShowTimeStamps(isShowTimeStamps)
{
}

FormatThread::~FormatThread(void)
{
	requestInterruption();
	while(m_isRunning)
		wait(1);
}

void FormatThread::run(void)
{
	m_isRunning = true;

	QString html;

	int pos = 0;
	int count = m_dataList.count();
	for(SrtItemPtr pItem : m_dataList)
	{
		html += "<div>";
		if(m_isShowTimeStamps)
		{
			html += QString("<p style='color: gray; font-size: 10pt; margin: 0; padding: 0;'>%1->%2</p>")
					.arg(pItem->startTimestamp.toString())
					.arg(pItem->stopTimestamp.toString());
		}
		html += "<p style='font-size: 11pt; margin: 0; padding: 0; margin-bottom: 6px;'>" + pItem->text + "</p>";
		html += "</div>";

		pos++;
		const double percent = pos / double(count) * 100;
		emit progressChanged(percent);
	}

	emit htmlReady(m_filePath, m_dataList, html);

	m_isRunning = false;
}
