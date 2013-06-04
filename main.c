/** @mainpage
    The Tetris is an implementation of the classic game for UEFI platform.
    I believe this Open Source application will be useful for students learning programming. 
    Dedicated to my little princess Maria. 
    
    @version    1.0
    @date       May 17, 2013
    @author     Vyacheslav Kowal
    
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

**/


#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include "Game.h"


/**
  The user Entry Point for Application. 
  The user code starts with this function as the real entry point for the application.
  @param  UINTN Argc     the arguments amount
  @param  CHAR16 **Argv  arguments list
  @return INTN 
**/
INTN ShellAppMain( UINTN Argc, CHAR16 **Argv )
{
    EFI_STATUS Status;
    EFI_SIMPLE_TEXT_OUTPUT_MODE SavedConsoleMode;
    GAME *Game = NULL;
    
    // Save the current console cursor position and attributes
    CopyMem(&SavedConsoleMode, gST->ConOut->Mode, sizeof(EFI_SIMPLE_TEXT_OUTPUT_MODE));
  
    Status = gST->ConOut->EnableCursor(gST->ConOut, FALSE);
    ASSERT_EFI_ERROR(Status);
    Status = gST->ConOut->ClearScreen(gST->ConOut);
    ASSERT_EFI_ERROR(Status);
    Status = gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));
    ASSERT_EFI_ERROR(Status);
    Status = gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
    ASSERT_EFI_ERROR(Status);
    
    // Initialize the Game object
    ConstructGame(&Game);

    // The main loop of the game
    while(Game->Status != EXIT_GAME) {
    
        // Parse a key press
        Game->ParseKeyPress(Game);
        
        // ReDraw the screen
        Game->ReDraw(Game);
    }

    // Finalize the Game object
    DestructGame(Game);
    
    // Restore the cursor visibility, position, and attributes
    gST->ConOut->EnableCursor(gST->ConOut, SavedConsoleMode.CursorVisible);
    gST->ConOut->SetCursorPosition(gST->ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
    gST->ConOut->SetAttribute(gST->ConOut, SavedConsoleMode.Attribute);
    gST->ConOut->ClearScreen(gST->ConOut);
    
    return 0;
}

