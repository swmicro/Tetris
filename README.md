Tetris
======

    The Tetris is an implementation of the classic game for UEFI platform.
    I believe this Open Source application will be useful for students learning programming. 
    Dedicated to my little princess Maria. 
    
    FEATURES
    - Simple Object Oriented structure 'Game -> Board -> Piece'
    - Doxygen complient
    - Periodic timer callback
    - Pseudorandom generator
    - Simple rotation alghorithm
    - No global variables
    - use __debugbreak(); for breakpoint in the debug mode
    
    
HOW TO INCORPORATE THIS APPLICATION INTO NT32

The instructions below are included as a sample and template on how a 
developer may integrate this code into an existing platform:

1. Copy files into ShellPkg folder: 
    \ShellPkg\Application\Tetris
    
2. Add this app build to the NT32 build:
   Add the Tetris.inf to the [components] section as it is in the ShellPkg.dsc:
    ShellPkg/Application/Tetris/Tetris.inf 

3. Find the [PcdsDynamicDefault.common.DEFAULT] section in the Nt32Pkg.dsc file and change the PcdWinNtFileSystem folder:
      gEfiNt32PkgTokenSpaceGuid.PcdWinNtFileSystem|L".!..\..\..\..\Build\Shell\DEBUG_MYTOOLS\IA32"|VOID*|106

4. Build Tetris under ShellPkg:
    build -p ShellPkg\ShellPkg.dsc -a IA32 -m ShellPkg\Application\Tetris\Tetris.inf

5. Build NT32
    build all

6. Run NT32
    build run
    
7. enter fsnt1: and execute Tetris.efi 

Enjoy!
