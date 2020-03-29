# WasmZip
WasmZip is a WebAssembly implementation of a data compression library for the zip format. It is a wrapper around miniz library written in C. It allows you to compress multiple files into a zip archive from a browser environment.
 
# Installation

## Requirements
Emscripten version 1.39.8 (or newer) is required to compile the project. 

## Steps

* Clone the WasmZip repository

  git clone https://github.com/bpajk/WasmZip.git

* Clone the miniz repository

  git clone https://github.com/kuba--/zip

* Build miniz library with emscripten

  cd zip

  mkdir build

  cd build

  emcmake cmake ..

  emmake make

* Build WasmZip

  cd "WasmZip src folder with main.cpp"

  * Development build

    em++ main.cpp ../zip/build/libzip.a -I ../zip/src -o ../docs/main.js -std=c++11 --bind -s ALLOW_MEMORY_GROWTH=1

  * Production build

    em++ main.cpp ../zip/build/libzip.a -I ../zip/src -o ../docs/main.js -std=c++11 --bind -s ALLOW_MEMORY_GROWTH=1 -O1
    
    NOTE: optimization arguments: "-O2", "-O3", "--closure 1" break the module

# Usage
WebAssembly module exposes a Zip class that can be used to compress multiple files into a zip archive.

## Class constructor

### Zip(zipFileName: string)
The constructor creates an instance of the Zip class
 * zipFileName - file name of the archive

## Methods

### addEntry(entryFileName: string, buffer: Uint8Array): void
Adds an entry (file) to the zip archive
 * entryFileName - file name of the entry
 * buffer - file data

### finish(): Int8Array
 * returns the zip archive data


### Example
A live example page can be found at:

https://bpajk.github.io/WasmZip

To run the example web page localy, you have to serve it with a web server. You can install a http-server with npm:

npm install http-server -g

Serve from the project source folder:

cd docs

http-server -p 8080

Open the example page in a web browser on the url: http://localhost:8080

## Credits
miniz library:

https://code.google.com/archive/p/miniz/

https://github.com/kuba--/zip

## License
WasmUtil is licensed under the MIT license.
