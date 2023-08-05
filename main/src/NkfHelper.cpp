#include "NkfHelper.h"

#include "nkf32.h"

namespace nkf {
	
	/**
	 * @brief  変換を行います
	 * @param  filePath  処理するファイル
	 * @param  cc        文字コード
	 * @param  lfc       改行コード
	 * @param  enableBOM BOMの有効化フラグ、Unicodeで有効
	 */
	bool convert( const QString& filePath, nkf::ECharacterCode cc, nkf::ELineFeedCode lfc, bool enableBOM  ) {
		QStringList option;
		switch( cc ) {
			case nkf::ECharacterCode::SJIS:
				option << "-s";
				break;
			case nkf::ECharacterCode::EUC:
				option << "-e";
				break;
			case nkf::ECharacterCode::UTF8:
				if( enableBOM ) {
					option << "-w8";
				}
				else {
					option << "-w80";
				}
				break;
			case nkf::ECharacterCode::UTF16LE:
				if( enableBOM ) {
					option << "-w16L";
				}
				else {
					option << "-w16L0";
				}
				break;
			case nkf::ECharacterCode::UTF16BE:
				if( enableBOM ) {
					option << "-w16B";
				}
				else {
					option << "-w16B0";
				}
				break;
		}

		switch( lfc ) {
			case nkf::ELineFeedCode::LF:
				//"-Lu"
				//SetNkfOption( (LPSTR) "-Lu" );
				option << "-Lu";
				break;
			case nkf::ELineFeedCode::CRLF:
				//SetNkfOption( (LPSTR) "-Lw" );
				option << "-Lw";
				break;
			case nkf::ELineFeedCode::CR:
				option << "-Lm";
				break;
		}
		char abuf[255];
		auto aaa = option.join( " " );
		//char* pChar = aaa.toStdString().data();
		strcpy( abuf, aaa.toStdString().c_str() );
		//char* pChar = aaa.toUtf8().data();
		SetNkfOption( abuf );

		QByteArray data = fs::readAll( filePath );

		int nsize = data.size() * 1.5;
		QByteArray buf( nsize, Qt::Uninitialized );
		DWORD ret;
		//char buffer[ data.size() ];
		BOOL returnCode = NkfConvertSafe( buf.data(), nsize, &ret, data.data(), data.size() );

		if( returnCode != TRUE ) return false;
		if( ret == 0 ) return false;

		QByteArray b( buf.data(), ret );
		fs::writeByteArray( filePath, b );

		return true;
	}

	bool convertEUC( const QString& filePath ) {
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

