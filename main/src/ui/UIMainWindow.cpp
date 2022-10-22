#include "UIMainWindow.h"
#include "ui_UIMainWindow.h"

#include "UIPanelMain.h"
#include "UIPanelConfig.h"
#include "UIPanelInfo.h"


//////////////////////////////////////////////////////////////////////////////////
class UIMainWindow::Impl : public Ui_UIMainWindow {
public:

	UIMainWindow* self;

	UIPanelMain* uiPanelMain;
	UIPanelConfig* uiPanelConfig;
	UIPanelInfo* uiPanelInfo;

	QMap<EPanelMode, QWidget*> map;


	/////////////////////////////////////////
	Impl( UIMainWindow* _self ) :
		self( _self ) {

		UIMainWindow::instance = self;
		setupUi( self );

		caption->attach( self );

		uiPanelMain = new UIPanelMain( self );
		verticalLayout_2->insertWidget( 0, uiPanelMain );

		uiPanelConfig = new UIPanelConfig( self );
		verticalLayout_2->insertWidget( 1, uiPanelConfig );

		uiPanelInfo = new UIPanelInfo( self );
		verticalLayout_2->insertWidget( 2, uiPanelInfo );

		map.insert( EPanelMode::Files, uiPanelMain );
		map.insert( EPanelMode::Config, uiPanelConfig );
		map.insert( EPanelMode::About, uiPanelInfo );

		
	}


	/////////////////////////////////////////
	void setup() {
		rollbackWindow();
	}


	/////////////////////////////////////////
	void backupWindow() {
		config.pos = self->pos();
		config.size = self->size();
	}


	/////////////////////////////////////////
	void rollbackWindow() {
		self->move( config.pos );
		self->resize( config.size );

		//$::restoreState( splitter, config.splitter );
	}


	/////////////////////////////////////////
	/// @brief  パネル表示の変更
	void changePanel( EPanelMode panelMode ) {
		for( auto k : map.keys() ) {
			map[ k ]->setVisible( k == panelMode );
		}
	}
};

UIMainWindow* UIMainWindow::instance;


//////////////////////////////////////////////////////////////////////////////////
UIMainWindow::UIMainWindow( QWidget* parent )
	: HFramelessWindow( parent ),
	impl( new Impl( this ) ) {

	connect( this, &UIMainWindow::signal_changePanel, [&]( EPanelMode panelMode ) {
		impl->changePanel( panelMode );
	} );
}


/////////////////////////////////////////
UIMainWindow::~UIMainWindow() {
}


/////////////////////////////////////////
void UIMainWindow::start() {
	impl->setup();

	emit signal_start();

	UIMainWindow::changePanel( EPanelMode::Files );
}


/////////////////////////////////////////
void UIMainWindow::closeEvent( QCloseEvent* /*event*/ ) {
	emit signal_closeWindow();
	impl->backupWindow();
}


/////////////////////////////////////////
void UIMainWindow::changePanel( EPanelMode panelMode ) {
	emit qtWindow->signal_changePanel( panelMode );
}
