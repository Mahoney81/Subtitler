#include "stdafx.h"
#include "MainWindow.h"
#include "TabPageWidget.h"
#include "LoadSrtThread.h"
#include "FormatThread.h"

MainWindow::MainWindow(void)
{
	m_ui.setupUi(this);

	auto onTabCloseRequestedFunc = [this](int index)
	{
		m_ui.pTabWidget->widget(index)->deleteLater();
		m_ui.pTabWidget->removeTab(index);
	};

	QTabBar *pTabBar = m_ui.pTabWidget->tabBar();
	pTabBar->setStyle(QStyleFactory::create("Fusion"));
	connect(pTabBar, &QTabBar::tabCloseRequested, this, onTabCloseRequestedFunc);

	restoreGeometry(QSettings().value("MainWindowGeometry").toByteArray());

	QStringList fileList = QSettings().value("LastOpenedFiles").toStringList();
	for(const QString& filePathEncoded : fileList)
	{
		const QString filePath = QString::fromUtf8(QByteArray::fromBase64(filePathEncoded.toUtf8()));
		loadFile(filePath, true);
	}
	if(fileList.isEmpty())
		m_ui.pTabWidget->addTab(new TabPageWidget(this), tr("Empty"));
}

MainWindow::~MainWindow(void)
{
	QSettings().setValue("MainWindowGeometry", saveGeometry());

	QStringList fileList;
	const int count = m_ui.pTabWidget->count();
	for(int i = 0; i < count; i++)
	{
		TabPageWidget *pPageWidget = qobject_cast<TabPageWidget*>(m_ui.pTabWidget->widget(i));
		Q_ASSERT(pPageWidget);
		const QString filePathEncoded = pPageWidget->filePath().toUtf8().toBase64();
		const int pos = pPageWidget->filePosition();
		QSettings().setValue(filePathEncoded + "_file_position", pos);
		fileList << filePathEncoded;
	}
	QSettings().setValue("LastOpenedFiles", fileList);
}

void MainWindow::changeEvent(QEvent *pEvent)
{
	QMainWindow::changeEvent(pEvent);
	switch (pEvent->type())
	{
	case QEvent::LanguageChange:
		m_ui.retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::on_pOpenAction_triggered(void)
{
	const QString lastFolder = QSettings().value("LastFolder").toString();
	const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Subtitles File"), lastFolder, tr("Subtitles Files (*.srt)"));
	if(!filePath.isEmpty())
	{
		const QFileInfo fi(filePath);
		QSettings().setValue("LastFolder", fi.absolutePath());
		loadFile(filePath);
	}
}

void MainWindow::on_pExitAction_triggered(void)
{
	close();
}

void MainWindow::on_pAboutQtAction_triggered(void)
{
	QMessageBox::aboutQt(this);
}

void MainWindow::on_pAboutAction_triggered(void)
{
	QString text = tr("<b>Subtitler</b><br>Version %1<br><br>A simple subtitles viewer.<br><br>Copyright © 2017 by <a href=\"https://ezsoftware.ru\">EzSoftware.Ru</a>. All rights reserved.").arg(QApplication::applicationVersion());
	QMessageBox::about(this, QApplication::applicationName(), text);
}

void MainWindow::onDataLoaded(const QString& filePath, const SrtItemPtrList& dataList)
{
	QProgressDialog *pProgressDialog = new QProgressDialog(tr("Formatting"), tr("Stop"), 0, 100, this);
	pProgressDialog->setMinimumDuration(0);
	pProgressDialog->setMinimumWidth(400);
	pProgressDialog->show();

	bool isShowTimeStamps = m_ui.pShowTimestampsAction->isChecked();

	FormatThread *pThread = new FormatThread(filePath, dataList, isShowTimeStamps, this);

	auto onProgressDialogCancelledFunc = [pThread, this](void)
	{
		if(pThread->isRunning())
		   m_ui.pStatusBar->showMessage(tr("Cancelled"));
		pThread->requestInterruption();
	};
	auto onProgressChangedFunc = [pProgressDialog](const int percent){ pProgressDialog->setValue(percent); };
	auto onThreadFinishedFunc = [pProgressDialog](void){pProgressDialog->close(); pProgressDialog->deleteLater(); };

	connect(pProgressDialog, &QProgressDialog::canceled, this, onProgressDialogCancelledFunc);
	connect(pThread, &FormatThread::progressChanged, this, onProgressChangedFunc);
	connect(pThread, &FormatThread::htmlReady, this, &MainWindow::onHtmlReady);
	connect(pThread, &FormatThread::finished, this, onThreadFinishedFunc);
	pThread->start();
	m_ui.pStatusBar->showMessage(tr("Formatting..."));
}

void MainWindow::onHtmlReady(const QString& filePath, const SrtItemPtrList& dataList, const QString& html)
{
	TabPageWidget *pWidget = qobject_cast<TabPageWidget*>(m_ui.pTabWidget->currentWidget());
	if(!pWidget)
	{
		pWidget = new TabPageWidget(this);
		m_ui.pTabWidget->addTab(pWidget, tr("Empty"));
	}
	if(!pWidget->isEmpty() && pWidget->filePath() != filePath)
	{
		pWidget = new TabPageWidget(this);
		int index = m_ui.pTabWidget->addTab(pWidget, tr("Untitled"));
		m_ui.pTabWidget->setCurrentIndex(index);
	}
	int index = m_ui.pTabWidget->currentIndex();
	m_ui.pTabWidget->setTabText(index, QFileInfo(filePath).fileName());
	pWidget->setHtml(filePath, dataList, html);

	const QString filePathEncoded = filePath.toUtf8().toBase64();
	int pos = QSettings().value(filePathEncoded + "_file_position", -1).toInt();
	if(pos > 0)
		pWidget->setFilePosition(pos);

	m_ui.pStatusBar->showMessage(tr("Ready"));
}

void MainWindow::reportError(const QString& errorText)
{
	DetailedMessageBox::critical(errorText);
}

void MainWindow::loadFile(const QString& filePath, bool silentMode)
{
	LoadSrtThread *pThread = new LoadSrtThread(filePath, this);

	if(!silentMode)
	{
		QProgressDialog *pProgressDialog = new QProgressDialog(tr("Loading the file"), tr("Stop"), 0, 100, this);
		pProgressDialog->setMinimumDuration(0);
		pProgressDialog->setMinimumWidth(400);
		pProgressDialog->show();

		auto onProgressDialogCancelledFunc = [pThread, this](void)
		{
			if(pThread->isRunning())
				m_ui.pStatusBar->showMessage(tr("Cancelled"));
			pThread->requestInterruption();

		};
		auto onProgressChangedFunc = [pProgressDialog](const int percent){ pProgressDialog->setValue(percent); };
		auto onThreadFinishedFunc = [pProgressDialog](void){pProgressDialog->close(); pProgressDialog->deleteLater(); };

		connect(pProgressDialog, &QProgressDialog::canceled, this, onProgressDialogCancelledFunc);
		connect(pThread, &LoadSrtThread::progressChanged, this, onProgressChangedFunc);
		connect(pThread, &LoadSrtThread::finished, this, onThreadFinishedFunc);
	}

	connect(pThread, &LoadSrtThread::dataLoaded, this, &MainWindow::onDataLoaded);
	connect(pThread, &LoadSrtThread::failed, this, &MainWindow::reportError);

	pThread->start();
	m_ui.pStatusBar->showMessage(tr("Loading..."));
}

void MainWindow::on_pNextPageAction_triggered(void)
{
	TabPageWidget *pWidget = qobject_cast<TabPageWidget*>(m_ui.pTabWidget->currentWidget());
	if(pWidget)
		pWidget->scrollToNextPage();
}

void MainWindow::on_pPreviousPageAction_triggered(void)
{
	TabPageWidget *pWidget = qobject_cast<TabPageWidget*>(m_ui.pTabWidget->currentWidget());
	if(pWidget)
		pWidget->scrollToPreviousPage();
}

void MainWindow::on_pShowTimestampsAction_triggered(bool isChecked)
{
	Q_UNUSED(isChecked);

	TabPageWidget *pWidget = qobject_cast<TabPageWidget*>(m_ui.pTabWidget->currentWidget());
	if(pWidget)
	{
		const QString filePath = pWidget->filePath();
		const SrtItemPtrList dataList = pWidget->cachedDataList();
		onDataLoaded(filePath, dataList);
	}
}
