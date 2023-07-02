#include "UIDialogConfig.h"
#include "ui_UIDialogConfig.h"

#include "NkfHelper.h"
#include "nkf32.h"



//////////////////////////////////////////////////////////////////////////////////
class UIDialogConfig::Impl : public Ui_UIDialogConfig {
public:

	UIDialogConfig* self;

	QMap<QListWidgetItem*, QWidget*> item2Widget;

	using InitData = std::tuple<QString, QWidget*>;

	/////////////////////////////////////////
	Impl( UIDialogConfig* _self ) :
		self( _self ) {

		setupUi( self );

		InitData init[] = {
			{u8"拡張子の設定",configSuffixEdit},
			{u8"除外フォルダーパス",ignoreFolder},
		};

		for( auto& i : init ) {
			auto* item = new QListWidgetItem( std::get<0>( i ) );
			listWidget->addItem( item );
			item2Widget.insert( item, std::get<1>( i ) );
		}

		float w = self->width();
		splitter->setSizes( { (int) ( w * 0.3f ), (int) ( w * 0.7f ) } );

		$ListWidget::itemSelectionChanged( listWidget, std::bind( &Impl::_itemSelectionChanged, this ) );

		listWidget->setCurrentRow( (int) config.lastConfigRow );
	}


	/////////////////////////////////////////
	void _itemSelectionChanged() {
		showWidget( listWidget->currentItem() );
		config.lastConfigRow = listWidget->currentRow();
	}


	/////////////////////////////////////////
	void showWidget( QListWidgetItem* e ) {
		for( auto& k : item2Widget.keys() ) {
			item2Widget[ k ]->setVisible( k == e );
		}
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIDialogConfig::UIDialogConfig( QWidget* parent ) :
	QDialog( parent, Qt::Tool ),
	impl( new Impl( this ) ) {
}


/////////////////////////////////////////
UIDialogConfig::~UIDialogConfig() {
}


/////////////////////////////////////////
//void UIDialogConfig::showEvent( QShowEvent* event ) {
//	impl->showWidget( impl->listWidget->currentItem() );
//}
