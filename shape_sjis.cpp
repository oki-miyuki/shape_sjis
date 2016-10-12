#include <fstream>
#include <iostream>
#include <iomanip>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/**
 * @file shape_sjis.cpp
 * Shapeファイルには、*.dbf ファイルの19byte目に、格納されている文字コードの情報が埋め込まれています。
 *   シフトJISコードで出力されている Shape ファイルの中には、この文字コードの情報が設定されていない
 *   場合が見受けられます。本プログラムは、これを強制的に修正し、シフトJISコードの情報を埋め込みます。
 *
 *  *.dbf ファイルをバイナリ・エディタで見た場合
 *                                         ★ この2行目が 0x13 がシフトJISを示すコードになります。
 *  03 74 09 1D 72 00 00 00 41 01 65 00 00 00 00 00 
 *  00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 00 
 *  49 44 00 00 00 00 00 00 00 00 00 43 00 00 00 00 
 *  04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 */

void show_usage() {
	std::cout << "---------------------------------------------------------------------" << std::endl;
	std::cout << "Shapeファイルの言語をSJISの設定に強制的に書き換えます。" << std::endl;
	std::cout << " データの元ディレクトリを指定すると、サブディレクトリも全部処理します" << std::endl;
	std::cout << "---------------------------------------------------------------------" << std::endl;
	std::cout << "shape_sjis [search_directory]" << std::endl;
	std::cout << "例:" << std::endl;
	std::cout << "C:\\Data> shape_sjis . " << std::endl;
}

template <typename T>
void traverse_directory( const fs::path& p, T func );
void rewrite_dbf_code( const std::string& fn, char code );

void rewrite_dbf( const fs::directory_iterator p ) {
      std::string ext   = (*p).path().extension().generic_string();
      std::string fname = (*p).path().generic_string();
      if( ext == ".dbf" ) {
        //std::cout << fname << std::endl;
        rewrite_dbf_code( fname, 0x13 );
      }
}

template <typename T>
void traverse_directory( const fs::path& p, T func ) {
  fs::directory_iterator de;
  fs::directory_iterator ds( p );
  for( fs::directory_iterator p = ds; p != de; ++p ) {
    if( fs::is_directory( *p ) ) {
      std::cout << std::endl << "D:" << *p << std::endl;
      traverse_directory( (*p).path(), func );
    } else {
      func( p );
    }
  }
}

void rewrite_dbf_code( const std::string& fn, char code ) {
	const char buf[1] = { code };
	std::fstream f;
	try {
		f.open( fn.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary );
		f.seekg( 29, std::ios::beg );
		f.write( buf, 1 );
		f.close();
		std::cout << ".";
	} catch( std::exception& e ) {
		std::cerr << e.what() << std::endl;
	}
}

int main(int argc, char* argv[]) {
	if( argc != 2 ) {
		show_usage();
		return 1;
	}
	traverse_directory( argv[1], rewrite_dbf );

	return 0;
}

