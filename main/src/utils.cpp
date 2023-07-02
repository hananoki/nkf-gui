#include "utils.h"
#include "UIMainWindow.h"

#include "ItemFile.h"
#include "ItemFolder.h"
#include "ActionFile.h"
#include "ActionFolder.h"
#include "ContextMenu.h"



namespace utils {

	/////////////////////////////////////////
	void createFileContextMenu( std::unique_ptr<ContextMenu>* contextMenu, HTreeWidget* treeWidget ) {
		$TreeWidget::customContextMenuRequested( treeWidget, [treeWidget, contextMenu]( const QPoint& pos ) {
			QModelIndex index = treeWidget->indexAt( pos );
			if( !index.isValid() ) return;

			auto* item = dynamic_cast<ItemFile*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
			if( item ) {
				( *contextMenu ) = std::make_unique<ContextMenu>();
				( *contextMenu )->addAction<ActionOpenFile>( treeWidget );
				( *contextMenu )->addAction<ActionShowInExplorer>( treeWidget );
				( *contextMenu )->addAction<ActionCopyFileName>( treeWidget );
			//	( *contextMenu )->addSeparator();
			//	( *contextMenu )->addAction<ActionOpenText>( treeWidget );
			//	( *contextMenu )->addAction<ActionOpenBSA>( treeWidget );
			//	( *contextMenu )->addAction<ActionOpenNIF>( treeWidget );
			//	( *contextMenu )->addAction<ActionOpenESP>( treeWidget );
				( *contextMenu )->exec( treeWidget->viewport()->mapToGlobal( pos ) );
			}
		} );
	}


	/////////////////////////////////////////
	void createFolderContextMenu( std::unique_ptr<ContextMenu>* contextMenu, HTreeWidget* treeWidget ) {
		$TreeWidget::customContextMenuRequested( treeWidget, [treeWidget, contextMenu]( const QPoint& pos ) {
			QModelIndex index = treeWidget->indexAt( pos );
			if( !index.isValid() ) return;

			auto* item = dynamic_cast<ItemFolder*>( reinterpret_cast<QTreeWidgetItem*>( index.internalPointer() ) );
			if( item ) {
				( *contextMenu ) = std::make_unique<ContextMenu>();
				( *contextMenu )->addAction<ActionOpenFolder>( treeWidget );
				( *contextMenu )->addAction<ActionFolderPathCopy>( treeWidget );
				( *contextMenu )->addSeparator();
				( *contextMenu )->addAction<ActionFolderAddIgnore>( treeWidget );

				( *contextMenu )->exec( treeWidget->viewport()->mapToGlobal( pos ) );
			}

		} );
	}

};


