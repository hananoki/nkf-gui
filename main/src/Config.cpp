#include "Config.h"
#include "json.h"
#include <magic_enum.hpp>

using namespace cpplinq;

DECLARE_ENUM( nkf::ECharacterCode )

Config config;

///////////////////////////////////////
void Config::load() {
	json::read( QCoreApplication::applicationName() + ".json", []( const QJsonObject& jsonObj ) {
#undef CONFIG_VALUE
#define CONFIG_VALUE(type,name) config. name = json::GetValue<type>( jsonObj[ #name ] );

		CONFIG_VALUE_LIST

	} );

	if( config.suffixes.isEmpty() ) {
		config.suffixes = QStringList{ "cpp","h","cs","cc" };
	}
	if( config.characterCode.isEmpty() ) {
		config.characterCode = "UTF8";
	}

	$::naturalSort( config.suffixes );
	$::naturalSort( config.ignorePaths );
}


///////////////////////////////////////
void Config::save() {
	json::write( QCoreApplication::applicationName() + ".json", []( QJsonObject& jsonObj ) {

#undef CONFIG_VALUE
#define CONFIG_VALUE(type,name) json::SetValue( jsonObj , #name, config. name );
		CONFIG_VALUE_LIST
	} );
}


///////////////////////////////////////
void Config::changedValue() {
	save();
}


///////////////////////////////////////
QString Config::suffixFilter() const {
	auto lst = from( config.suffixes )
		>> select( []( const QString& x ) { return $$( "*.%1" ).arg( x ); } )
		>> to_qlist();
	return lst.join( ";" );
}


///////////////////////////////////////
bool Config::hasSuffix( QString filePath ) const {
	return config.suffixes.contains( path::getSuffix( filePath ) );
}


///////////////////////////////////////
nkf::ECharacterCode Config::getCharacterCode() const {
	auto e = magic_enum::enum_cast<nkf::ECharacterCode>( config.characterCode.toStdString() );
	return e.value();
}


///////////////////////////////////////
nkf::ELineFeedCode Config::getLineFeedCode() const {
	auto e = magic_enum::enum_cast<nkf::ELineFeedCode>( config.lineFeedCode.toStdString() );
	return e.value();
}


///////////////////////////////////////
bool Config::containsIgnorePath( QString folderPath ) const {
	for( auto& f : config.ignorePaths ) {
		if( folderPath.contains( f ) )return true;
	}
	return false;
}

///////////////////////////////////////
void Config::addIgnorePath( QString folderPath ) {
	if( 0 <= ignorePaths.indexOf( folderPath ) )return;
	ignorePaths.append( folderPath );
	emit viewStateChange();
}

