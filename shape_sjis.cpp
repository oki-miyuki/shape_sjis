#include <fstream>
#include <iostream>
#include <iomanip>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/**
 * @file shape_sjis.cpp
 * Shape�t�@�C���ɂ́A*.dbf �t�@�C����19byte�ڂɁA�i�[����Ă��镶���R�[�h�̏�񂪖��ߍ��܂�Ă��܂��B
 *   �V�t�gJIS�R�[�h�ŏo�͂���Ă��� Shape �t�@�C���̒��ɂ́A���̕����R�[�h�̏�񂪐ݒ肳��Ă��Ȃ�
 *   �ꍇ�����󂯂��܂��B�{�v���O�����́A����������I�ɏC�����A�V�t�gJIS�R�[�h�̏��𖄂ߍ��݂܂��B
 *
 *  *.dbf �t�@�C�����o�C�i���E�G�f�B�^�Ō����ꍇ
 *                                         �� ����2�s�ڂ� 0x13 ���V�t�gJIS�������R�[�h�ɂȂ�܂��B
 *  03 74 09 1D 72 00 00 00 41 01 65 00 00 00 00 00 
 *  00 00 00 00 00 00 00 00 00 00 00 00 00 13 00 00 
 *  49 44 00 00 00 00 00 00 00 00 00 43 00 00 00 00 
 *  04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 */

void show_usage() {
	std::cout << "---------------------------------------------------------------------" << std::endl;
	std::cout << "Shape�t�@�C���̌����SJIS�̐ݒ�ɋ����I�ɏ��������܂��B" << std::endl;
	std::cout << " �f�[�^�̌��f�B���N�g�����w�肷��ƁA�T�u�f�B���N�g�����S���������܂�" << std::endl;
	std::cout << "---------------------------------------------------------------------" << std::endl;
	std::cout << "shape_sjis [search_directory]" << std::endl;
	std::cout << "��:" << std::endl;
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

