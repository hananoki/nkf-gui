#include "UIMainWindow.h"
#include "ui_UIMainWindow.h"

//#include "UIPanelMain.h"
#include "UIDialogConfig.h"
#include "UIDialogInfo.h"

#include "NkfHelper.h"

#include "utils.h"
#include "ItemFile.h"
#include "ItemFolder.h"
#include "ContextMenu.h"

#include <QPainter>

using FileFunc = std::function<void( ItemFile* item )>;
using FolderFunc = std::function<void( ItemFolder* item )>;

//////////////////////////////////////////////////////////////////////////////////
class RequestData {
public:
	QString fullPath;
};

//////////////////////////////////////////////////////////////////////////////////
class UIMainWindow::Impl : public Ui_UIMainWindow, public HWorker<RequestData> {
public:
	using self_t = UIMainWindow;
	self_t* self;

	std::unique_ptr<ContextMenu> contextMenuFolder;

	/////////////////////////////////////////
	Impl( UIMainWindow* _self ) :
		self( _self ) {

		self_t::instance = self;
		setupUi( self );

		treeWidget->setAcceptDrops( true );
		treeWidget->setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );
		treeWidget->installEventFilter( self );

		if( !config.lastInput.isEmpty() ) {
			registerFolderPath( config.lastInput );
		}

		connect( &watcher, &QFutureWatcher<void>::finished, self, [&]() {
			updateItemState();
			UIStatusBar::info( u8"ファイルのリストアップが完了しました" );
		} );

		connect(
			&watcher,
			&QFutureWatcher<void>::progressTextChanged,
			self,
			std::bind( &Impl::watcher_progressTextChanged, this, std::placeholders::_1 ) );
	}

	/////////////////////////////////////////
	void watcher_progressTextChanged( const QString& progressText ) {
		UIStatusBar::info( progressText );
	}

	/////////////////////////////////////////
	void setup() {
		rollbackWindow();
		utils::createFileContextMenu( &contextMenuFolder, treeWidget );
		utils::createFolderContextMenu( &contextMenuFolder, treeWidget );

		$Action::triggered( actionSettings, std::bind( &self_t::showDialog<UIDialogConfig>, self ) );
		$Action::triggered( actionInfo, std::bind( &self_t::showDialog<UIDialogInfo>, self ) );

		// ウィンドウを閉じた時
		connect( mainWindow, &UIMainWindow::signal_closeWindow, std::bind( &Impl::_closeWindow, this ) );

		// アイテム追加
		connect( self, &self_t::addTreeItem, self, std::bind( &Impl::_addTreeItem, this, std::placeholders::_1 ) );

		connect( commandBar, &UICommandBar::buttonConvert, std::bind( &Impl::convert, this ) );
		connect( commandBar, &UICommandBar::viewStateChange, std::bind( &Impl::updateItemState, this ) );
		connect( commandBar, &UICommandBar::selectFileFlag, std::bind( &Impl::setCheckState, this, std::placeholders::_1 ) );

		connect( &config, &Config::viewStateChange, std::bind( &Impl::updateItemState, this ) );

		$TreeWidget::itemDoubleClicked( treeWidget, std::bind( &Impl::_itemDoubleClicked, this, std::placeholders::_1, std::placeholders::_2 ) );

		startWorker();
	}


	/////////////////////////////////////////
	virtual void handleWorker( const RequestData& data ) override {
		if( fs::isExistFile( data.fullPath ) ) {
			if( config.hasSuffix( data.fullPath ) ) {
				emit self->addTreeItem( new ItemFile( data.fullPath ) );
			}
		}
		else {
			QString folderPath = data.fullPath;

			emit self->addTreeItem( new ItemFolder( folderPath ) );

			fs::enumerateDirectories( folderPath, "*", SearchOption::AllDirectories, [&]( QString s )->bool {
				//if( !config.containsIgnorePath( s ) ) {
				watcher.progressTextChanged( s );

				emit self->addTreeItem( new ItemFolder( s ) );
				//}
				return true;
			} );
		}

	}


	/////////////////////////////////////////
	void registerFolderPath( QStringList lst, bool clear = true ) {
		if( clear ) {
			treeWidget->clear();
		}

		UIStatusBar::info( u8"ファイルのリストアップ処理中", -1 );

		for( auto& path : lst ) {
			addRequest( RequestData{ path } );
		}
	}


	/////////////////////////////////////////
	void traverseItems( FileFunc func ) {
		for( auto& top : treeWidget->topLevelItems<QTreeWidgetItem>() ) {//
			auto* folder = dynamic_cast<ItemFolder*>( top );
			if( folder ) {
				for( auto& child : folder->childItems<ItemFile>() ) {
					func( child );
				}
				continue;
			}

			auto* file = dynamic_cast<ItemFile*>( top );
			if( file ) {
				func( file );
				continue;
			}
		}
	}


	/////////////////////////////////////////
	void traverseItems( FolderFunc func ) {
		for( auto& top : treeWidget->topLevelItems<ItemFolder>() ) {
			func( top );
		}
	}



	/////////////////////////////////////////
	void _closeWindow() {
		if( future.isRunning() ) {
			setCancelQueue();
			workerExit = true;
			condRequest.wakeAll();
			future.waitForFinished();
		}
	}



	/////////////////////////////////////////
	void _itemDoubleClicked( QTreeWidgetItem* item, int /*column*/ ) {
		auto* file = dynamic_cast<ItemFile*>( item );
		if( file ) {
			$::showInExplorer( file->fullPath );
		}
	}


	/////////////////////////////////////////
	void _addTreeItem( QTreeWidgetItem* item ) {
		if( !item )return;

		treeWidget->addTopLevelItem( item );

		if( dynamic_cast<ItemFolder*>( item ) ) {

			auto* folder = reinterpret_cast<ItemFolder*>( item );

			for( auto& child : folder->childItems<ItemFile>() ) {
				//func( child );
				child->updateItemState();
			}

			folder->setExpanded( true );
			folder->updateVisible();
			
		}
	}


	/////////////////////////////////////////
	void backupWindow() {
		config.pos = self->pos();
		config.size = self->size();
		$::saveState( config.fileTreeHeader, treeWidget->header() );
	}


	/////////////////////////////////////////
	void rollbackWindow() {
		self->move( config.pos );
		self->resize( config.size );
		$::restoreState( treeWidget->header(), config.fileTreeHeader );
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
	void updateItemState() {
		traverseItems( [&]( ItemFile* item ) {
			item->updateItemState();
		} );
		traverseItems( [&]( ItemFolder* item ) {
			item->updateVisible();
		} );
	}


	/////////////////////////////////////////
	void convert() {
		int count = 0;
		traverseItems( [&]( ItemFile* item ) {
			if( item->checkState( 0 ) != Qt::Checked )return;

			nkf::convertUTF8( item->fullPath );
			item->updateItemState( true );
			count++;
		} );

		if( 0 < count ) {
			UIStatusBar::info( $$( tr( u8"%1ファイル 変換しました" ).arg( count ) ) );
		}
		else {
			UIStatusBar::info( $$( tr( u8"変換するファイルはありません" ) ) );
		}
	}


	/////////////////////////////////////////
	void setCheckState( bool state ) {
		traverseItems( [&]( ItemFile* item ) {
			if( item->isHidden() )return;
			item->setCheckState( 0, state ? Qt::Checked : Qt::Unchecked );
		} );
	}

};


UIMainWindow* UIMainWindow::instance;


//////////////////////////////////////////////////////////////////////////////////
UIMainWindow::UIMainWindow( QWidget* parent ) :
	QMainWindow( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UIMainWindow::~UIMainWindow() {
}


/////////////////////////////////////////
void UIMainWindow::start() {
	impl->setup();

	emit signal_start();
}


/////////////////////////////////////////
//void UIMainWindow::paintEvent( QPaintEvent* pe ) {
//	QPixmap pixmap;
//	pixmap.load( ":/res/image/Haloween_Yuko.png" );
//	QPainter paint( this );
//	int widWidth = impl->centralwidget->width() / 2;
//	int widHeight = impl->centralwidget->height() / 2;
//	pixmap = pixmap.scaled( widWidth , widHeight , Qt::KeepAspectRatio );
//	paint.drawPixmap( 1 +  widWidth , 2 +  widHeight  - 10, pixmap );
//	QWidget::paintEvent( pe );
//}


/////////////////////////////////////////
void UIMainWindow::closeEvent( QCloseEvent* /*event*/ ) {
	emit signal_closeWindow();
	impl->backupWindow();
}


/////////////////////////////////////////
/// @brief  イベントフィルター
bool UIMainWindow::eventFilter( QObject* /*obj*/, QEvent* event ) {

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

