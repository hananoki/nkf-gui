#include "ActionFolder.h"
#include "ItemFolder.h"
#include "UIMainWindow.h"

//////////////////////////////////////////////////////////////////////////////////
ActionOpenFolder::ActionOpenFolder( HTreeWidget* treeWidget ) :
	QAction( tr( u8"フォルダを開く" ) ) {

	setIcon( ICON_EXPLORER );

	connect( this, &QAction::triggered, [treeWidget]() {
		auto* item = treeWidget->currentItem<ItemFolder>();
		if( item ) {
			item->openFolder();
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );
}


//////////////////////////////////////////////////////////////////////////////////
ActionFolderPathCopy::ActionFolderPathCopy( HTreeWidget* treeWidget ) :
	QAction( tr( u8"フォルダーパスをクリップボードにコピー" ) ) {

	//setIcon( ICON_EXPLORER );

	connect( this, &QAction::triggered, [treeWidget]() {
		auto* item = treeWidget->currentItem<ItemFolder>();
		if( item ) {
			QClipboard* clipboard = QApplication::clipboard();
			clipboard->setText( path::separatorToOS( item->fullPath ) );
		}
		else {
			HLogView::error( u8"選択アイテムエラー" );
		}
	} );

}


//////////////////////////////////////////////////////////////////////////////////
ActionFolderAddIgnore::ActionFolderAddIgnore( HTreeWidget* treeWidget ) :
	QAction( tr( u8"除外フォルダーパスに追加する" ) ) {

	//setIcon( ICON_EXPLORER );

	connect( this, &QAction::triggered, [treeWidget]() {
		auto* item = treeWidget->currentItem<ItemFolder>();
		if( item ) {
			config.addIgnorePath( item->fullPath );
		}
		//else {
		//	HLogView::error( u8"選択アイテムエラー" );
		//}
	} );

}

