#pragma once

//////////////////////////////////////////////////////////////////////////////////
class Item_File : public QTreeWidgetItem {
public:
	QString fullPath;
	nkf::ECharacterCode code;

	/////////////////////////////////////////
	Item_File( const QString& _fullPath ) :
		fullPath( _fullPath ) {

		setText( 0, path::getFileName( fullPath ) );
		setIcon( 0, icon::get( fullPath ) );


		setCheckState( 0, Qt::Unchecked );

		updateCodeState( true );
	}


	/////////////////////////////////////////
	void updateCodeState( bool read = false ) {
		if( read ) {
			code = nkf::getCharacterCode( fullPath );
			setText( 1, nkf::toString( code ) );
		}
		if( code != config.getCharacterCode() ) {
			setBackgroundColor( 0, "#FCC" );
			setBackgroundColor( 1, "#FCC" );
			setHidden( false );
		}
		else {
			setData( 0, Qt::BackgroundColorRole, QColor( Qt::GlobalColor::transparent ) );
			setData( 1, Qt::BackgroundColorRole, QColor( Qt::GlobalColor::transparent ) );
			if( config.ignoreFile ) {
				setHidden(true);
			}
			else {
				setHidden( false );
			}
		}
	}
};
