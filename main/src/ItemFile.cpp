#include "ItemFile.h"

//////////////////////////////////////////////////////////////////////////////////
ItemFile::ItemFile( const QString& _fullPath ) :
	fullPath( _fullPath ) {

	setText( 0, path::getFileName( fullPath ) );
	setIcon( 0, icon::get( fullPath ) );

	setCheckState( 0, Qt::Unchecked );

	updateItemState( true );
}


/////////////////////////////////////////

/**
 * @brief  アイテムの表示状態の更新
 * @param  trueならnkfを使用して文字コードを探す
 */
void ItemFile::updateItemState( bool read /*= false*/ ) {
	if( read ) {
		code = nkf::getCharacterCode( fullPath );
		setText( 1, nkf::toString( code ) );
	}

	bool hide = false;

	// 文字コードが違う場合
	if( code != config.getCharacterCode() ) {
		setBackgroundColor( 0, "#FCC" );
		setBackgroundColor( 1, "#FCC" );
		//setHidden( false );
	}
	// 文字コードが同じ場合
	else {
		setData( 0, Qt::BackgroundColorRole, QColor( Qt::GlobalColor::transparent ) );
		setData( 1, Qt::BackgroundColorRole, QColor( Qt::GlobalColor::transparent ) );

		// 不要ファイル除外
		if( config.ignoreFlag ) {
			hide = true;
		}
	}

	// ASCIIの除外
	if( config.ignoreAsciiFlag && code == nkf::ECharacterCode::ASCII ) {
		hide = true;
	}

	setHidden( hide );
}


/////////////////////////////////////////
QString ItemFile::fileName() {
	return path::getFileName( fullPath );
}


/////////////////////////////////////////
QString ItemFile::folderPath() {
	return path::getDirectoryName( fullPath );
}


/////////////////////////////////////////
bool ItemFile::showInExplorer() {
	auto d = folderPath();
	if( !fs::isExistDirectory( d ) )return false;
	$::showInExplorer( d );
	return true;
}

/////////////////////////////////////////
bool ItemFile::openFile() {
	if( !fs::isExistFile( fullPath ) ) {
		HLogView::error( u8"ファイルが見つかりません: " + fullPath );
		return false;
	}
	$::showInExplorer( fullPath );
	return true;
}
