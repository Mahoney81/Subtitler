#include "stdafx.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("Subtitler");
	a.setOrganizationName("EzSoftware");
	a.setApplicationVersion("0.1.0");
	a.setAttribute(Qt::AA_UseHighDpiPixmaps);

	QString profile = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
	if(!QDir().exists(profile))
		QDir().mkpath(profile);

	QTranslator qtTranslator;
	qtTranslator.load(":/translations/qtbase_" + QLocale::system().name());
	a.installTranslator(&qtTranslator);

	QTranslator appTranslator;
	appTranslator.load(":/translations/subtitler_" + QLocale::system().name());
	a.installTranslator(&appTranslator);

	qRegisterMetaType<SrtItemPtrList>("SrtItemPtrList");

	a.setWindowIcon(QIcon(":/icons/subtitles-icon"));

	try
	{
		MainWindow w;
		w.show();

		return a.exec();
	}
	catch(ExceptionPtr pException)
	{
		DetailedMessageBox::critical(QObject::tr("Oops... We've crashed :("),
									 QObject::tr("Something went wrong... Please, re-run the app and if problem persists, contact the developer."),
									 pException);
		return 1;
	}
}
