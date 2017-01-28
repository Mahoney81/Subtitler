#include "stdafx.h"
#include "TabPageWidget.h"

TabPageWidget::TabPageWidget(QWidget *pParent) : QWidget(pParent)
{
	m_ui.setupUi(this);
	m_ui.pSearchEdit->hide();
	m_ui.pSeconderyTextBrowser->hide();
}

void TabPageWidget::setHtml(const QString& filePath, const SrtItemPtrList& dataList, const QString& html)
{
	m_filePath = filePath;
	m_dataList = dataList;
	m_ui.pMainTextBrowser->setHtml(html);
	m_isEmpty = false;
}

void TabPageWidget::changeEvent(QEvent *pEvent)
{
	QWidget::changeEvent(pEvent);
	switch (pEvent->type()) {
	case QEvent::LanguageChange:
		m_ui.retranslateUi(this);
		break;
	default:
		break;
	}
}
