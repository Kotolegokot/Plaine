# PlaneRunner
Fuck you bitch motherfucker

## Dependencies
* [Irrlicht](http://irrlicht.sourceforge.net)
* [bullet](bulletphysics.org)
* [Freetype](http://gnuwin32.sourceforge.net/packages/freetype.htm)
* [zlib](http://www.zlib.net/) (as a dependency of Freetype)
* [gettext](http://ftp.gnu.org/pub/gnu/gettext/)
* [Code::Blocks](http://codeblocks.org) (optional, but strongly recommended under Windows)

## Building
Simply open the project in Code::Blocks, choose the appropriate build target and press Ctrl-F9. Libs must be put into `deps/lib/` (i.e. `deps/lib/Irrlicht.dll`) directory, and their headers into `deps/include/` (i.e. `deps/include/irrlicht/`). Linux standard paths (`/usr/lib/`, `/usr/include/` etc.) work as well.

Also you can use `cmake` to build the project (doesn't work on Windows yet (mb)):
* `mkdir build`
* `cd build`
* `cmake .. [-DDEBUG=1]`
* `make [-jN]` where N is the number of your processor cores

You can change the compiler used by specifying the CXX variable.

QtCreator works fine, too.

### Compiler
TDM-GCC (Windows), gcc (linux), clang (linux) have been tested to successfully bulid the project.
