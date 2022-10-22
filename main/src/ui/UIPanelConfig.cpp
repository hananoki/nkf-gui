#include "UIPanelConfig.h"
#include "ui_UIPanelConfig.h"

#include "NkfHelper.h"
#include "nkf32.h"



//////////////////////////////////////////////////////////////////////////////////
class UIPanelConfig::Impl : public Ui_UIPanelConfig {
public:

	UIPanelConfig* self;

	QMap<EConfigItem, QWidget*> map;

	class Item : public QListWidgetItem {
	public:
		Item( QString t, EConfigItem e ) : QListWidgetItem(), e( e ) {
			setText( t );
			//setIcon( QIcon(":/res/icon/f_f_health_50_s16_f_health_50_1bg.png") );
			setIcon( icon::FA::gear() );
		}
		EConfigItem e;
	};

	/////////////////////////////////////////
	Impl( UIPanelConfig* _self ) :
		self( _self ) {

		//UIFileList::instance = self;
		setupUi( self );

		listWidget->addItem( new Item( tr( u8"拡張子の設定" ), EConfigItem::SuffixEdit ) );
		listWidget->addItem( new Item( tr( u8"除外フォルダーパス" ), EConfigItem::IgnoreFolder ) );

		map.insert( EConfigItem::SuffixEdit, configSuffixEdit );
		map.insert( EConfigItem::IgnoreFolder, ignoreFolder );

		//showWidget( config.lastEConfigItem );
		listWidget->setCurrentRow( (int) config.lastEConfigItem );

		float w = self->width();
		splitter->setSizes( { (int) ( w * 0.3f ), (int) ( w * 0.7f ) } );

		$ListWidget::itemSelectionChanged( listWidget, [&]() {
			auto* item = (Item*) listWidget->currentItem();
			showWidget( item->e );
			config.lastEConfigItem = item->e;
		} );
	}

	/////////////////////////////////////////
	void showWidget( EConfigItem  e ) {
		for( auto& k : map.keys() ) {
			map[ k ]->setVisible( k == e );
		}
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIPanelConfig::UIPanelConfig( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {
}


/////////////////////////////////////////
UIPanelConfig::~UIPanelConfig() {
}

/////////////////////////////////////////
void UIPanelConfig::showEvent( QShowEvent* event ) {
	impl->showWidget( config.lastEConfigItem );
}
