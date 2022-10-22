#include "UICommandBar.h"
#include "ui_UICommandBar.h"

#include "UIMainWindow.h"
#include <magic_enum.hpp>
#include "NkfHelper.h"
#include "Item_File.h"
#include "UIPanelMain.h"



//////////////////////////////////////////////////////////////////////////////////
class UICommandBar::Impl : public Ui_UICommandBar {
public:

	UICommandBar* self;

	/////////////////////////////////////////
	Impl( UICommandBar* _self ) :
		self( _self ) {

		//UIFileList::instance = self;
		setupUi( self );
		//comboBox->setProperty( "text-align", "center" );
		auto lst = $enums::names<nkf::ECharacterCode>();
		$ComboBox::addItems( comboBox, lst );

		comboBox->setCurrentIndex( lst.indexOf( config.characterCode ) );

		checkBox->setChecked( config.ignoreFile );
	}


	/////////////////////////////////////////
	void setup() {

		$ComboBox::currentIndexChanged( comboBox, [&]( int index ) {
			config.characterCode = comboBox->currentText();
			UIPanelMain::traverseItems( [&]( Item_File* item ) {
				item->updateCodeState();
			} );
		} );

		// すべて選択
		$PushButton::clicked( button_all, [&]( bool ) {
			UIPanelMain::traverseItems( [&]( Item_File* item ) {
				if( item->isHidden() )return;
				item->setCheckState( 0, Qt::Checked );
			} );
		} );

		// すべて選択
		$PushButton::clicked( button_uncheck, [&]( bool ) {
			UIPanelMain::traverseItems( [&]( Item_File* item ) {
				if( item->isHidden() )return;
				item->setCheckState( 0, Qt::Unchecked );
			} );
		} );

		$PushButton::clicked( button_convert, [&]( bool ) {
			int count = 0;
			UIPanelMain::traverseItems( [&]( Item_File* item ) {
				if( item->checkState( 0 ) != Qt::Checked )return;

				nkf::convertUTF8( item->fullPath );
				item->updateCodeState( true );
				count++;
			} );

			if( 0 < count ) {
				UIStatusBar::info( $$( tr( u8"%1ファイル 変換しました" ).arg( count ) ) );
			}
			else {
				UIStatusBar::info( $$( tr( u8"変換するファイルはありません" ) ) );
			}
		} );

		$CheckBox::stateChanged( checkBox, [&]( int state ) {
			config.ignoreFile = state;
			UIPanelMain::traverseItems( [&]( Item_File* item ) {
				item->updateCodeState();
			} );
		} );
	}
};



//////////////////////////////////////////////////////////////////////////////////
UICommandBar::UICommandBar( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		impl->setup();
	} );
}

/////////////////////////////////////////
UICommandBar::~UICommandBar() {
}
