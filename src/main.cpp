/*
 * main.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: bpajk
 */

/**
 * # Install procedure
 * # Compiled with emscripten 1.39.8
 *
 * git clone https://github.com/kuba--/zip.git
 * cd zip
 *
 * mkdir build
 * cd build
 * emcmake cmake .. # Use CMake to generate Makefile
 * emmake make # build the project
 *
 * cd <main.cpp source folder>
 *
 * # Development build
 * em++ main.cpp ../zip/build/libzip.a -I ../zip/src -o ../docs/main.js -std=c++11 --bind -s ALLOW_MEMORY_GROWTH=1
 *
 *
 * # Production build
 * # NOTE: optimization arguments: "-O2", "-O3", "--closure 1" break the module
 * em++ main.cpp ../zip/build/libzip.a -I ../zip/src -o ../docs/main.js -std=c++11 --bind -s ALLOW_MEMORY_GROWTH=1 -O1
 *
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
// Zip library header
#include <zip.h>
// Emscripten headers
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace std;
using namespace emscripten;

class Zip {
public:
  Zip(std::string zipFileName){
	  cout << "Zip constructor" << endl;
	  //set the archive path
	  zipFilePath= "/";
	  //append the archive filename
	  zipFilePath.append(zipFileName);
	  //create a zip archive
	  zip = zip_open(
			  	  zipFilePath.c_str(),
				  ZIP_DEFAULT_COMPRESSION_LEVEL, 'w'
			  );
	  archiveBuffer = NULL;
	  archiveBufferLength = 0;
  }

  ~Zip(){
	  cout << "Zip destructor" << endl;
	  if(archiveBuffer != NULL){
		  delete[] archiveBuffer;
	  }
  }

  /**
   * Add an entry to the zip archive
   */
  void addEntry(std::string entryFileName, std::string buffer){
	  //open a zip entry
	  zip_entry_open(zip, entryFileName.c_str());
	  {
		  cout << "addEntry " << entryFileName << " Size: " << buffer.length() << " B" << endl;
		  //write the buffer to the zip entry
	      zip_entry_write(zip, buffer.c_str(), buffer.length());
	  }
	  //close the zip entry
	  zip_entry_close(zip);
  }

  val finish(){
	  //close the zip archive
	  zip_close(zip);
	  //check if zip archive exists
	  ifstream f(zipFilePath);
	  if(f.good()){
		  std::ifstream t;
		  //open the archive
		  t.open(zipFilePath);
		  //go to the end
		  t.seekg(0, std::ios::end);
		  //report location (this is the length)
		  archiveBufferLength = t.tellg();
		  //go back to the beginning
		  t.seekg(0, std::ios::beg);
		  //allocate memory for a buffer of appropriate dimension
		  archiveBuffer = new char[archiveBufferLength];
		  //read the whole file into the buffer
		  t.read(archiveBuffer, archiveBufferLength);
		  //close the archive
		  t.close();
		  cout << "Archive successfully read. Size: " << archiveBufferLength << " B" << endl;
	  }else{
		  throw std::runtime_error("Error: archive not found\n");
	  }

	  //remove the zip file
	  if( remove(zipFilePath.c_str()) == 0 ){
		  cout << "Archive successfully deleted" << endl;
	  }else{
		  throw std::runtime_error("Error: there was a problem deleting the archive\n");
	  }

	  //return fileContent;
	  return val(typed_memory_view(archiveBufferLength, archiveBuffer));
	}

private:
  std::string zipFilePath;
  struct zip_t* zip;
  char* archiveBuffer;
  int archiveBufferLength;
};


// Binding code
EMSCRIPTEN_BINDINGS(zip_bindings) {
  class_<Zip>("Zip")
    .constructor<std::string>()
    .function("addEntry", &Zip::addEntry)
	.function("finish", &Zip::finish)
    ;
}


int main(){}
