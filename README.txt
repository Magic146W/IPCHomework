#FileTransfer - Client–Server File Sender using Boost.Asio

A simple C++ console application for transfering a file over TCP from client to server (run localy) using Boost.Asio

## Features

- File is specified through client command-line.
- File is sent over TCP to the server.
- Server receives and stores the file.
- Error handling and good resource management practices.
- Works with `localhost`, good as a demo or to build on.

## Requirements

- CMake 3.10+
- Boost 1.70+ (headers + built Boost.System lib)
- C++17
- Visual Studio / Rider / VS Code (Windows) or g++/clang (Linux)

## Boost Instructions
###If boost installed

(In case of having boost already) Program uses version boost_1_87_0, by use of stiff path in CMake file. 
Path can be changed but Boost version cannot be older than version 1.70 (check requirements).

###If no Boost!

1. dowload boost 1.87.0 and unzip it at -> "C:\Program Files\boost\boost_1_87_0"
2. using cmd:
  2.1. cd C:\Program Files\boost\boost_1_87_0
  2.2. bootstrap.bat
  2.3. b2 --with-system stage

## Build Instructions  (powershell)

1. Download the ZIP file.
2. Unzip to your desired folder.
3. Open the folder in Visual Studio Code.
	From the VSCode terminal run:
cd <your-unzipped-folder>
mkdir build
cd build
cmake ..
cmake --build .

## Run instructions (Server must be running before starting the client, so follow steps)

	From the VSCode terminal run (if in main folder, otherwise you can skip "\build" and do ".\Debug\..."):
1. .\build\Debug\server.exe
2. .\build\Debug\client.exe <your path to file>
3. Your file should be in build folder :D