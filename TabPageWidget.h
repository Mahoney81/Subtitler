#ifndef TABPAGEWIDGET_H
#define TABPAGEWIDGET_H
#include <QtWidgets>
#include "ui_TabPageWidget.h"
#include "SrtItem.h"

class TabPageWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TabPageWidget(QWidget *pParent);

	void setHtml(const QString& filePath, const SrtItemPtrList& dataList, const QString& html);
	bool isEmpty(void) const { return m_isEmpty; }		// nothing has been loaded yet

	const QString& filePath(void) const { return m_filePath; }
	int filePosition(void) const { return m_ui.pMainTextBrowser->verticalScrollBar()->value(); }

	void setFilePosition(int pos) { m_ui.pMainTextBrowser->verticalScrollBar()->setValue(pos); }

	void scrollToNextPage(void) { m_ui.pMainTextBrowser->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd); }
	void scrollToPreviousPage(void) { m_ui.pMainTextBrowser->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub); }

	const SrtItemPtrList& cachedDataList(void) const { return m_dataList; }

protected:
	void changeEvent(QEvent *pEvent);

private:
	Ui::TabPageWidget m_ui;
	bool m_isEmpty = true;
	QString m_filePath;
	SrtItemPtrList m_dataList; // cache
};

#endif // TABPAGEWIDGET_H
