#include "UIConfigIgnoreFolder.h"
#include "ui_UIConfigIgnoreFolder.h"

//////////////////////////////////////////////////////////////////////////////////
class UIConfigIgnoreFolder::Impl : public Ui_UIConfigIgnoreFolder {
public:

	UIConfigIgnoreFolder* self;

	class Item : public QListWidgetItem {
	public:
		Item( QString path ) : QListWidgetItem() {
			setText( path );
			setIcon( icon::get( path ) );
		}
	};

	/////////////////////////////////////////
	Impl( UIConfigIgnoreFolder* _self ) :
		self( _self ) {

		setupUi( self );

		listWidget->setAcceptDrops( true );
		listWidget->installEventFilter( self );

		//button_m->setIcon( icon::FA::minus() );

		makeIgnoreList();

		$PushButton::clicked( button_m, [&]( bool ) {
			int index = listWidget->currentRow();
			if( index < 0 ) {
				UIStatusBar::warning( tr( u8"アイテムを選択してください" ) );
				return;
			}
			auto* item = listWidget->takeItem( index );
			config.ignorePaths.removeOne( item->text() );
		} );

		//connect( &config, &Config::signal_changeIgnorePath, [&]() {
		//	makeIgnoreList();
		//} );
	}


	/////////////////////////////////////////
	void makeIgnoreList() {
		listWidget->clear();
		for( auto& f : config.ignorePaths ) {
			listWidget->addItem( new Item( f ) );
		}
	}

	/////////////////////////////////////////
	bool dropEvent( QDropEvent* ee ) {
		using namespace cpplinq;

		auto lst = from( ee->mimeData()->urls() )
			>> select( []( QUrl x ) { return QUrl( x ).toLocalFile(); } )
			>> to_qlist();

		for( auto& f : lst ) {
			if( fs::isExistFile( f ) )continue;
			config.ignorePaths.append( f );

			listWidget->addItem( new Item( f ) );
		}

		return true;
	}

	
};



//////////////////////////////////////////////////////////////////////////////////
UIConfigIgnoreFolder::UIConfigIgnoreFolder( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {
}

/////////////////////////////////////////
UIConfigIgnoreFolder::~UIConfigIgnoreFolder() {
}


/////////////////////////////////////////
/// @brief  イベントフィルター
bool UIConfigIgnoreFolder::eventFilter( QObject* /*obj*/, QEvent* event ) {

	if( event->type() == QEvent::Drop ) {
		QDropEvent* ee = (QDropEvent*) event;
		if( !ee->mimeData()->hasUrls() ) return false;

		return impl->dropEvent( ee );
	}

	if( event->type() == QEvent::DragEnter ) {
		QDragEnterEvent* ee = (QDragEnterEvent*) event;
		ee->acceptProposedAction();
		return true;
	}

	if( event->type() == QEvent::DragMove ) {
		QDragMoveEvent* ee = (QDragMoveEvent*) event;
		ee->acceptProposedAction();
		return true;
	}
	return false;
}


