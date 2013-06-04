/** @file
    Copyright (C) 2013 Vyacheslav Kowal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include "game.h"



/**
  This function refreshes the game action.
  @param GAME *This    pointer to the GAME stucture.  
**/
VOID ReDraw(GAME *This)
{
    // redraw the screen
    This->Board->ReDrawBoard(This->Board);
    
    if (This->GameTimer == 0 && This->Status != GAME_OVER) {
        This->GameTimer -= This->Level;
        if (This->Board->MovePieceDown(This->Board) == FALSE)
            if (This->Board->NextPiece(This->Board) == FALSE)
                ; //This->Status = GAME_OVER;
        This->GameTimer = GAME_REFRESH_PERIOD; 
    }
}


/**
  Parsing a keypress
  @param GAME *This    The pointer to the GAME stucture.  
**/
VOID ParseKeyPress(GAME *This)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    
    Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return;
    
    switch(Key.ScanCode) 
    {
        case SCAN_LEFT:
            This->Board->MovePieceLeft(This->Board);
        break;
        
        case SCAN_RIGHT:
            This->Board->MovePieceRight(This->Board);
        break;
        
        case SCAN_UP:
            This->Board->RotatePiece(This->Board);
        break;
        
        case SCAN_DOWN:
            This->Board->MovePieceDown(This->Board);
        break;
        
        case SCAN_PAGE_UP:
            This->Level++;
        break;
        
        case SCAN_PAGE_DOWN: 
            if (This->Level) 
                This->Level--;
        break;
        
        case SCAN_ESC:
            This->Status = EXIT_GAME;
        break;
        
        default:  break;
    }
    
    switch(Key.UnicodeChar) 
    {
        case L' ': This->Board->DropPiece(This->Board); break;
        case L'p':
        case L'P': This->PauseGame ^= This->PauseGame; break;
        default:  break;
    }
}


/**
  Callback on timer event.
  @param[in] EFI_EVENT Event
  @param[in] VOID *Context
**/
VOID TimerCallback( EFI_EVENT Event, VOID *Context )
{
    GAME *Game = (GAME*)Context;
    
    if (Game->GameTimer)
        Game->GameTimer--;
}


/**
  The GAME structure constructor.
  @param GAME *This    The pointer to the GAME stucture.  
**/
VOID ConstructGame(GAME **This)
{
    EFI_STATUS Status;
    
    GAME *Game = AllocatePool(sizeof(GAME));
    ZeroMem(Game, sizeof(GAME));
    
    Game->Status        = PLAY_GAME;
    Game->Score         = 0;
    Game->Level         = 1;
    Game->ReDraw        = ReDraw;
    Game->ParseKeyPress = ParseKeyPress;
    Game->GameTimer     = GAME_REFRESH_PERIOD;
    ConstructBoard(&Game->Board);
    
    Status = gBS->CreateEventEx( 
        EVT_TIMER | EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        TimerCallback, 
        Game, 
        NULL,
        &Game->TimerEvent
    );
    ASSERT_EFI_ERROR(Status);
    
    // Start the Periodic Timer 
    Status = gBS->SetTimer(Game->TimerEvent, TimerPeriodic, TIMER_PERIOD_INIT); 
    ASSERT_EFI_ERROR(Status);
    *This = Game;
}


/**
  The GAME destructor.
  @param GAME *This    The pointer to the GAME stucture.  
**/
VOID DestructGame(GAME *This)
{
    EFI_STATUS Status;
    Status = gBS->CloseEvent(This->TimerEvent);
    ASSERT_EFI_ERROR(Status);
    DestructBoard(This->Board);
    FreePool(This);
}
