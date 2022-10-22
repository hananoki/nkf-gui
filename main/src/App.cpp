#include "App.h"
#include "UIMainWindow.h"

//#include "AppHelper.h"
#include "HFusionStyle.h"



//////////////////////////////////////////////////////////////////////////////////
class App::Impl {
public:
	App* self;


	///////////////////////////////////////////
	Impl( App* _self )
		: self( _self ) {

	}


	/////////////////////////////////////////
	~Impl() {
	}


	/////////////////////////////////////////
	bool loadCurrentLangTranslator( QTranslator& translator ) {
		QString current_langfile = "qt_ja";
		if( fs::isExistFile( "translations/qt_ja.qm" ) ) {
			if( !translator.load( current_langfile, "translations" ) ) {
				assert( false );
				return false;
			}
		}
		return true;
	}


	/////////////////////////////////////////
	int run() {
		//QtFusionStyle::set();
		qApp->setStyle( QStyleFactory::create( "Fusion" ) );

		icon::initQtAwesome( 0.9f, "#000" );

		QTranslator t;
		loadCurrentLangTranslator( t );
		//t.load( "translations/qt_ja" );
		self->installTranslator( &t );

		// translate --->
		QTranslator translator;
		translator.load( "translations/main_ja" ); // 日本語の場合
		// translator.load("translate/qcss_fr"); // フランス語の場合はこうする
		self->installTranslator( &translator );
		// <---


		UIMainWindow w;

		QKeySequence keys_refresh( QKeySequence::Refresh );
		QAction* shortcutF5 = new QAction( &w );
		shortcutF5->setShortcut( keys_refresh );
		w.addAction( shortcutF5 );
		connect( shortcutF5, &QAction::triggered, []() {
			qApp->setStyleSheet( fs::readAllText( "style.qss" ) );
			UIStatusBar::info( tr( u8"スタイルシートを更新" ) );
		} );

		w.start();

		qApp->setStyleSheet( fs::readAllText( "style.qss" ) );

		w.show();

		return self->exec();
	}


};


App* App::instance;

//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////
App::App( int& argc, char** argv, int /*= ApplicationFlags*/ ) :
	QApplication( argc, argv ),
	impl( new Impl( this ) ) {

	instance = this;
	config.load();
}


/////////////////////////////////////////
App::~App() {
	config.save();
}

//////////////////////////////////////////////////////////////////////////////////
void logHandler( QtMsgType type, const QMessageLogContext& context, const QString& msg ) {
	const auto& message = qFormatLogMessage( type, context, msg );
	QTextStream cerr( stderr );
	cerr << message << endl;
	QFile file( "App.log" );
	if( !file.open( QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text ) ) {
		cerr << "Cannot open log file:" << file.fileName();
		return;
	}
	QTextStream( &file ) << message << endl;
}

int main( int argc, char* argv[] ) {
	qInstallMessageHandler( logHandler );
	qSetMessagePattern( "[%{time yyyy-MM-dd hh:mm:ss}] [%{category}] [%{threadid}] %{file}:%{line} => %{message} " );
	App app( argc, argv );
	return app.impl->run();
}

