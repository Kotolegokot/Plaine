# PlaneRunner
Fuck you bitch motherfucker

## Dependencies
* [Irrlicht](http://irrlicht.sourceforge.net)
* [Freetype](http://gnuwin32.sourceforge.net/packages/freetype.htm)
* [iconv](http://gnuwin32.sourceforge.net/packages/libiconv.htm)
* [Code::Blocks](http://codeblocks.org) (optional, but strongly recommended under Windows)

## Building
Simply open the project in Code::Blocks, choose the appropriate build target and press Ctrl-F9. Libs must be put into `lib/` (i.e. `lib/Irrlicht.dll`) directory, and their headers into `include\` (i.e. `include\irrlicht`). Under Linux standard paths (`/usr/lib/`, `/usr/include/` etc.) work as well.

Also you can use `automake` to build the project. The following targets are available:
* `make debug`
* `make release`
* `make clean`
* `make all` : both `debug` and `release`

You can change the compiler used by specifying the CXX variable (i.e. `make release CXX=clang++`). The default one is `g++`.
