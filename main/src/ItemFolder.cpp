#include "ItemFolder.h"
#include "ItemFile.h"

//////////////////////////////////////////////////////////////////////////////////
ItemFolder::ItemFolder( const QString& _fullPath ) :
	fullPath( _fullPath ) {

	setText( 0, fullPath );
	setIcon( 0, icon::get( fullPath ) );

	QList<QTreeWidgetItem*> lst;

	fs::enumerateFiles( fullPath, config.suffixFilter(), SearchOption::TopDirectoryOnly, [&lst]( const QString& s ) {
		lst << new ItemFile( s );
		return true;
	} );

	addChildren( lst );
}


/////////////////////////////////////////
void ItemFolder::openFolder() {
	$::showInExplorer( fullPath );
}


/////////////////////////////////////////
void ItemFolder::updateVisible() {
	if( childCount() == 0 ) {
		setHidden( true );
	}
	else {
		bool hide = true;
		for(auto& c: childItems<ItemFile>() ) {
			if( !c->isHidden() ) {
				hide = false;
				break;
			}
		}
		setHidden( hide );
	}

	if( config.containsIgnorePath( fullPath ) ) {
		setHidden( true );
	}
}


