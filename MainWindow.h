#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets>
#include "ui_MainWindow.h"
#include "SrtItem.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(void);
	virtual ~MainWindow(void);

protected:
	virtual void changeEvent(QEvent *pEvent) override;

private slots:
	void on_pOpenAction_triggered(void);
	void on_pExitAction_triggered(void);
	void on_pAboutQtAction_triggered(void);
	void on_pAboutAction_triggered(void);
	void on_pNextPageAction_triggered(void);
	void on_pPreviousPageAction_triggered(void);
	void on_pShowTimestampsAction_triggered(bool isChecked);

	void onDataLoaded(const QString& filePath, const SrtItemPtrList& dataList);
	void onHtmlReady(const QString& filePath, const SrtItemPtrList& dataList, const QString& html);
	void reportError(const QString& errorText);

private:
	Ui::MainWindow m_ui;

	void loadFile(const QString& filePath, bool silentMode = false);
};

#endif // MAINWINDOW_H
