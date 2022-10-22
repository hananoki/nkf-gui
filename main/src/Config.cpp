#include "Config.h"
#include "json.h"
#include <magic_enum.hpp>

using namespace cpplinq;

DECLARE_ENUM( nkf::ECharacterCode )
DECLARE_ENUM( EConfigItem )
/*
namespace json {
	template<>
	inline TMap GetValue<TMap>( const QJsonValue& value ) {
		TMap lst;
		for( QJsonValue s : value.toArray() ) {
			lst.insert( s[ "key" ].toString(), s[ "value" ].toString() );
		}
		return 	lst;
	}

	template<>
	inline void SetValue<TMap>( QJsonObject& jsonObj, const QString& name, const TMap& value ) {
		QJsonArray jsonArr;
		for( auto& k : value.keys() ) {
			QJsonObject jsObj;
			jsObj[ "key" ] = k;
			jsObj[ "value" ] = value[ k ];
			jsonArr.append( jsObj );
		}
		jsonObj[ name ] = jsonArr;
	}
}
*/

Config config;

///////////////////////////////////////
void Config::load() {
	json::read( QCoreApplication::applicationName() + ".json", []( const QJsonObject& jsonObj ) {
#undef CONFIG_VALUE
#undef CONFIG_VALUE_INIT
#define CONFIG_VALUE(type,name) config. name = json::GetValue<type>( jsonObj[ #name ] );
#define CONFIG_VALUE_INIT(type,name,ini) name = json::GetValue<type>( jsonObj[ #name ] );

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
#undef CONFIG_VALUE_INIT
#define CONFIG_VALUE(type,name) json::SetValue( jsonObj , #name, config. name );
#define CONFIG_VALUE_INIT(type,name,ini) json::SetValue( jsonObj , #name, config. name );

		CONFIG_VALUE_LIST
	} );
}


///////////////////////////////////////
QString Config::suffixFilter() {
	auto lst = from( config.suffixes )
		>> select( []( const QString& x ) { return $$( "*.%1" ).arg( x ); } )
		>> to_qlist();
	return lst.join( ";" );
}


///////////////////////////////////////
bool Config::hasSuffix( QString filePath ) {
	return config.suffixes.contains( path::getSuffix( filePath ) );
}


///////////////////////////////////////
nkf::ECharacterCode Config::getCharacterCode() {
	auto e = magic_enum::enum_cast<nkf::ECharacterCode>( config.characterCode.toStdString() );
	return e.value();
}

bool Config::containsIgnorePath( QString folderPath ) {
	for( auto& f : config.ignorePaths ) {
		if( folderPath.contains( f ) )return true;
	}
	return false;
}


void Config::addIgnorePath( QString folderPath ) {
	if( 0 <= ignorePaths.indexOf( folderPath ) )return;
	ignorePaths.append( folderPath );
	emit signal_changeIgnorePath();
}

