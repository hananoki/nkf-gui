#include "UIPanelMain.h"
#include "ui_UIPanelMain.h"

#include "UIMainWindow.h"
#include "NkfHelper.h"
#include "Item_File.h"

#define ICON_EXPLORER icon::get( QString( "C:/Windows/explorer.exe" ) )

//////////////////////////////////////////////////////////////////////////////////
class RequestData {
public:
	QString fullPath;
};



//////////////////////////////////////////////////////////////////////////////////
class Item_Folder : public HTreeWidgetItem {
public:
	QString fullPath;

	Item_Folder( const QString& _fullPath ) :
		fullPath( _fullPath ) {

		setText( 0, fullPath );
		setIcon( 0, icon::get( fullPath ) );

		QList<QTreeWidgetItem*> lst;
		for( auto& f : fs::getFiles( fullPath, config.suffixFilter(), SearchOption::TopDirectoryOnly ) ) {
			lst << new Item_File( f );
		}
		addChildren( lst );
	}

	//QList<QTreeWidgetItem*> childItems() {
	//	QList<QTreeWidgetItem*> lst;
	//	for( int i = 0; i < childCount(); i++ ) {
	//		lst << dynamic_cast<QTreeWidgetItem*>( child( i ) );
	//	}
	//	return lst;
	//}
};


//////////////////////////////////////////////////////////////////////////////////
class UIPanelMain::Impl : public Ui_UIPanelMain, public HWorker<RequestData> {
public:
	static UIPanelMain* instance;
	UIPanelMain* self;

	QMenu* contextMenu;

	/////////////////////////////////////////
	Impl( UIPanelMain* _self ) :
		self( _self ) {

		instance = self;
		setupUi( self );

		treeWidget->setAcceptDrops( true );
		treeWidget->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );
		treeWidget->installEventFilter( self );

		if( !config.lastInput.isEmpty() ) {
			registerFolderPath( config.lastInput );
		}
	}


	/////////////////////////////////////////
	void setup() {
#pragma region contextMenu
		contextMenu = new QMenu();
		contextMenu->addAction( $Action::create( tr( u8"エクスプローラで表示" ), ICON_EXPLORER, [&]() {
			auto* file = treeWidget->currentItem<Item_File>();
			if( file ) {
				$::showInExplorer( file->fullPath );
			}
			auto* folder = treeWidget->currentItem<Item_Folder>();
			if( folder ) {
				$::showInExplorer( folder->fullPath );
			}
		} ) );
		contextMenu->addSeparator();
		contextMenu->addAction( $Action::create( tr( u8"除外フォルダーパスに追加する" ), icon::FA::plus(), [&]() {
			auto* item = treeWidget->currentItem<Item_Folder>();
			if( item ) {
				config.addIgnorePath( item->fullPath );
				UIPanelMain::traverseItems( [&]( Item_Folder* folder ) {
					if( config.containsIgnorePath( folder->fullPath ) ) {
						folder->setHidden( true );
					}
				} );
			}
		} ) );

#pragma endregion

		QObject::connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			UIPanelMain::traverseItems( [&]( Item_File* item ) {
				item->updateCodeState();
			} );
		} );

		// ウィンドウを閉じた時
		connect( qtWindow, &UIMainWindow::signal_closeWindow, [&]() {
			if( future.isRunning() ) {
				setCancelQueue();
				workerExit = true;
				condRequest.wakeAll();
				future.waitForFinished();
			}
		} );

		// アイテム追加
		connect( self, &UIPanelMain::addTreeItem, self, [&]( QTreeWidgetItem* item ) {
			treeWidget->addTopLevelItem( item );
			if( dynamic_cast<Item_Folder*>( item ) ) {
				item->setExpanded( true );
				if( item->childCount() == 0 ) {
					item->setHidden( true );
				}
			}
		} );

		$TreeWidget::customContextMenuRequested( treeWidget, [&]( const QPoint& pos ) {
			QModelIndex index = treeWidget->indexAt( pos );
			if( !index.isValid() ) return;

			auto* folder = dynamic_cast<Item_Folder*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
			if( folder ) {
				contextMenu->exec( treeWidget->viewport()->mapToGlobal( pos ) );
			}
		} );

		$TreeWidget::itemDoubleClicked( treeWidget, [&]( QTreeWidgetItem* item, int /*column*/ ) {
			auto* file = dynamic_cast<Item_File*>( item );
			if( file ) {
				$::showInExplorer( file->fullPath );
			}
		} );

		startWorker();
	}


	/////////////////////////////////////////
	virtual void handleWorker( const RequestData& data ) override {
		if( fs::isExistFile( data.fullPath ) ) {
			if( config.hasSuffix( data.fullPath ) ) {
				emit self->addTreeItem( new Item_File( data.fullPath ) );
			}
		}
		else {
			QString folderPath = data.fullPath;

			if( !config.containsIgnorePath( folderPath ) ) {
				emit self->addTreeItem( new Item_Folder( folderPath ) );

				fs::enumerateDirectories( folderPath, "*", SearchOption::AllDirectories, [&]( QString s )->bool {
					if( !config.containsIgnorePath( s ) ) {
						emit self->addTreeItem( new Item_Folder( s ) );
					}
					return true;
				} );
			}
		}

	}


	/////////////////////////////////////////
	void registerFolderPath( QStringList lst, bool clear = true ) {
		if( clear ) {
			clearFileList();
		}

		for( auto& path : lst ) {
			addRequest( RequestData{ path } );
		}
	}


	/////////////////////////////////////////
	void clearFileList() {
		treeWidget->clear();
	}


	/////////////////////////////////////////
	bool dropEvent( QDropEvent* ee ) {
		using namespace cpplinq;

		auto lst = from( ee->mimeData()->urls() )
			>> select( []( QUrl x ) { return QUrl( x ).toLocalFile(); } )
			>> to_qlist();

		registerFolderPath( lst );
		config.lastInput = lst;
		return true;
	}


	/////////////////////////////////////////
	void traverseItems( FileFunc func ) {
		for( auto& top : treeWidget->topLevelItems<QTreeWidgetItem>() ) {//
			auto* folder = dynamic_cast<Item_Folder*>( top );
			if( folder ) {
				for( auto& child : folder->childItems<Item_File>() ) {
					func( child );
				}
				continue;
			}

			auto* file = dynamic_cast<Item_File*>( top );
			if( file ) {
				func( file );
				continue;
			}
		}
	}

	/////////////////////////////////////////
	void traverseItems( FolderFunc func ) {
		for( auto& top : treeWidget->topLevelItems<Item_Folder>() ) {
			func( top );
		}
	}

};

UIPanelMain* UIPanelMain::Impl::instance;



//////////////////////////////////////////////////////////////////////////////////
UIPanelMain::UIPanelMain( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect( qtWindow, &UIMainWindow::signal_start, [&]() {
		$::restoreState( impl->treeWidget->header(), config.fileTreeHeader );
		impl->setup();
	} );
}


/////////////////////////////////////////
UIPanelMain::~UIPanelMain() {
	$::saveState( config.fileTreeHeader, impl->treeWidget->header() );
}


/////////////////////////////////////////
/// @brief  イベントフィルター
bool UIPanelMain::eventFilter( QObject* /*obj*/, QEvent* event ) {

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


/////////////////////////////////////////
void UIPanelMain::traverseItems( FileFunc func ) {
	UIPanelMain::Impl::instance->impl->traverseItems( func );
}

/////////////////////////////////////////
void UIPanelMain::traverseItems( FolderFunc func ) {
	UIPanelMain::Impl::instance->impl->traverseItems( func );
}
