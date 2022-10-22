#include "UIMainCaption.h"
#include "ui_UIMainCaption.h"

#include "UIMainWindow.h"

#include "QtAwesome/QtAwesome.h"

//////////////////////////////////////////////////////////////////////////////////
class UIMainCaption::Impl : public Ui_UIMainCaption {
public:
	UIMainCaption* self;

	QMap<EPanelMode, HFlatPushButton*> map;

	QMenu* contextMenu;

	/////////////////////////////////////////
	Impl( UIMainCaption* self ) :self( self ) {
		setupUi( self );

#pragma region window button
		QVariantMap variantMap;
		variantMap.insert( "color", "#000" );
		variantMap.insert( "color-selected", "#000" );
		variantMap.insert( "color-disabled", "#666" );
		variantMap.insert( "color-active", "#000" );
		variantMap.insert( "scale-factor", 0.5 );

		titile_min_button->setIcon( icon::awesome->icon( fa::windowminimize, variantMap ) );
		connect( titile_min_button, &QPushButton::clicked, []( bool ) {
			qtWindow->showMinimized();
		} );

		titile_max_button->setIcon( icon::awesome->icon( fa::windowmaximize, variantMap ) );
		titile_max_button->setDisabled( true );

		titile_close_button->setIcon( icon::awesome->icon( fa::close, variantMap ) );
		connect( titile_close_button, &QPushButton::clicked, []( bool ) {
			qtWindow->close();
		} );
		//title_icon->setPixmap( QPixmap( ":/res/icon/f_f_health_50_s16_f_health_50_1bg.png" ) );
#pragma endregion

		button_about->setIcon( icon::FA::infocircle() );
		button_settings->setIcon( icon::FA::gears() );
		button_files->setIcon( icon::FA::listul() );

		map.insert( EPanelMode::Files, button_files );
		map.insert( EPanelMode::Config, button_settings );
		map.insert( EPanelMode::About, button_about );
	}



	/////////////////////////////////////////
	void setup() {

		$PushButton::clicked( button_files, [&]( bool ) {
			UIMainWindow::changePanel( EPanelMode::Files );
		} );

		$PushButton::clicked( button_settings, [&]( bool ) {
			UIMainWindow::changePanel( EPanelMode::Config );
		} );

		$PushButton::clicked( button_about, [&]( bool ) {
			UIMainWindow::changePanel( EPanelMode::About );
		} );
	}



	/////////////////////////////////////////
	void changePanel( EPanelMode panelMode ) {
		HSignalBlocker _( self );

		for( auto k : map.keys() ) {
			map[ k ]->setChecked( k == panelMode );
		}
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIMainCaption::UIMainCaption( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {


}


/////////////////////////////////////////
UIMainCaption::~UIMainCaption() {}





/////////////////////////////////////////
void UIMainCaption::attach( HFramelessWindow* window ) {

	window->addCaptionWidget( impl->title_icon );
	window->addCaptionWidget( impl->title_label );
	window->addCaptionWidget( impl->widget_L );
	window->addCaptionWidget( impl->widget_R );
	window->addCaptionWidget( impl->widget_C );
	window->addCaptionWidget( this );

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		impl->setup();
	} );

	connect( qtWindow, &UIMainWindow::signal_changePanel, [&]( EPanelMode panelMode ) {
		impl->changePanel( panelMode );
	} );

}


