## libramfs

### Informations  
This library implements an embedded filesystem mountpoint for usage with devkitPro. The implementations works by statically linking directories ramfs compressed with ustar to the executable.

### Usage
#### Library usage
Include library's header: `#include <ramfs.h>`
Call `ramfsInit()` at the start of your app and `ramfsExit()` before exiting

#### Makefile
To generate the ramfs, define a RAMFS_DIR variable in you makefile containing the path of your ramfs folder:

    RAMFS_DIR := ramfs_example_folder

Then, update your makefile to include ramfs.mk and add ramfs target and flags
(chage the example according to your makefile, replace TARGET_PLATFORM_HERE with wiiu or switch):

    include $(DEVKITPRO)/portlibs/TARGET_PLATFORM_HERE/share/ramfs.mk
    OFILES		+=	$(RAMFS_TARGET)
    CFLAGS		+=	-I$(DEVKITPRO)/portlibs/TARGET_PLATFORM_HERE/include
    CXXFLAGS	+=	-I$(DEVKITPRO)/portlibs/TARGET_PLATFORM_HERE/include
    LIBS		+=	-lramfs

#### cmake
Include ramfs's cmake file:

    include("${DEVKITPRO}/portlibs/wiiu/share/ramfs.cmake" REQUIRED)

Then, call ramfs_add to build and link the ramfs (make sure it's after all your_project add_executable calls):

    ramfs_add(your_project "ramfs_example_folder")

### Installing
A prebuild version for wiiu is available at the wiiu-fling pacman repository.
Please reffer to [these](https://gitlab.com/QuarkTheAwesome/wiiu-fling) instructions to set up wiiu-fling. 

To manually install the library:

Select your target plaftorm (replace TARGET_PLATFORM_HERE with wiiu or switch)

    $ export PLATFORM=TARGET_PLATFORM_HERE

Clone, build and install the library

    $ git clone https://gitlab.com/4TU/resinfs.git
    $ cd resinfs
    $ make
    $ sudo make install
