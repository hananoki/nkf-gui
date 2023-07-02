#include "NkfHelper.h"

#include "nkf32.h"

namespace nkf {
	bool convertEUC( const QString& filePath ) {
		return true;
	}

	bool convertSJIS( const QString& filePath ) {
		return true;
	}

	bool convertUTF8( const QString& filePath ) {
		// -g: 自動判別の結果を出力する。
		// -t: 何もしない。
		char opt[] = "-w8";
		SetNkfOption( opt );

		QByteArray data = fs::readAll( filePath );

		int nsize = data.size() * 1.5;
		QByteArray buf( nsize, Qt::Uninitialized );
		DWORD ret;
		//char buffer[ data.size() ];
		BOOL returnCode = NkfConvertSafe( buf.data(), nsize, &ret, data.data(), data.size() );

		if( returnCode != TRUE ) return false;
		if( ret == 0) return false;

		QByteArray b( buf.data(), ret );
		fs::writeByteArray( filePath , b );

		return true;
	}


	ECharacterCode getCharacterCode( const QString& filePath ) {
		char opt[] = "-g2 -t -d";
		SetNkfOption( opt );

		auto data = fs::readAll( filePath );

		DWORD ret;
		char buffer[ 64 ];
		BOOL returnCode = NkfConvertSafe( buffer, 64, &ret, data.data(), data.size() );
		//NkfFileConvert1( filePath.toStdString().c_str() );
		wchar_t wc[255];
		DWORD dw;
		GetNkfGuessW(wc,255,&dw );
		auto enc = (ECharacterCode) NkfGetKanjiCode();

		// US-ASCII も ISO-2022-JP と判別される。ISO-2022-JP であれば ESC (0x1B) が含まれるので・・・
		if( enc == ECharacterCode::JIS ) {
			enc = ECharacterCode::ASCII;  // US-ASCII と仮定する
			for( int i = 0; i < data.size(); i++ ) {
				if( data[ i ] == (char)0x1b ) {
					enc = ECharacterCode::JIS;
					break;
				}
			}
		}

		return enc;
	}
	const QStringList CharacterCode = QStringList()
		<< "Shift JIS"
		<< "EUC-JP"
		<< "ISO-2022-JP"
		<< "UTF-8"
		<< "UTF-16LE"
		<< "UTF-16BE"
		<< "ASCII"
		;
	const QString& toString( ECharacterCode  code ) {
		if( code == ECharacterCode::ASCII )return CharacterCode[ 6 ];
		return CharacterCode[ (int) code ];
	}
}

