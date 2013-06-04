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
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include "Board.h"



/**
  This function checks if current location is free 
  @param BOARD *This    pointer to the BOARD stucture.  
  @param BODY *Body pointer to future body
  @retval TRUE space is free
  @retval FALSE space occupied
**/
BOOLEAN CheckSpace(BOARD *This, BODY *Body, UINTN X, UINTN Y)
{
    UINTN  i;
    UINTN  x, y;
    
    for (i=0; i<4; i++) {
        x = Body->a[i][0] + X;
        y = Body->a[i][1] + Y;
        if ( This->Field[x][y] != EFI_TEXT_ATTR(EFI_BLACK, EFI_BLACK) )
            return FALSE;
    }
    return TRUE;
}

///    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0); Print(L"%d, %d",x,y);

/**
  This function refreshes the board field.
  @param BOARD *This    pointer to the BOARD stucture.  
**/
VOID ReDrawBoard(BOARD *This)
{
    CHAR16 ChrBlock[] = {BLOCKELEMENT_FULL_BLOCK,0};
    static UINT8  Buffer[BOARD_HEIGHT][BOARD_WIDTH] = {0};
    UINTN  x, y;
    
    for (x=0; x<BOARD_WIDTH; x++) 
        for (y=0; y<BOARD_HEIGHT; y++) 
            if (Buffer[x][y] != This->Field[x][y]) {
                gST->ConOut->SetCursorPosition(gST->ConOut, x + BOARD_LTOP_X, y + BOARD_LTOP_Y);
                gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(This->Field[x][y], EFI_BLACK));
                gST->ConOut->OutputString(gST->ConOut, ChrBlock);
                Buffer[x][y] = This->Field[x][y];
            }
}

    
/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
VOID ClearRows(BOARD *This)
{
}


/**
  This function erases the Piece at preset location 
  @param BOARD *This - pointer to the BOARD stucture.  
**/
VOID ErasePiece(BOARD *This)
{
    UINTN  i;
    UINTN  x, y;
    
    // Redraw the piece with black space
    for (i=0; i<4; i++) {
        x = This->Piece->Body.a[i][0] + This->Piece->X;
        y = This->Piece->Body.a[i][1] + This->Piece->Y;
        This->Field[x][y] = EFI_TEXT_ATTR(EFI_BLACK, EFI_BLACK);
    }
}


/**
  Draw the piece
  @param BOARD *This    pointer to the BOARD stucture.  
**/
BOOLEAN DrawPiece(BOARD *This)
{
    UINTN  i;
    UINTN  x, y;
    
    if (CheckSpace(This, &This->Piece->Body, This->Piece->X, This->Piece->Y) == FALSE) 
        return FALSE;
    
    for (i=0; i<4; i++) {
        x = This->Piece->X + This->Piece->Body.a[i][0];
        y = This->Piece->Y + This->Piece->Body.a[i][1];
        This->Field[x][y] = EFI_TEXT_ATTR(This->Piece->Color, EFI_BLACK);
    }
    return TRUE;
}


/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
VOID DropPiece(BOARD *This)
{
    BOOLEAN Result; 
    do {
        ErasePiece(This);
        Result = CheckSpace(This, &This->Piece->Body, This->Piece->X, This->Piece->Y+1);
        if (Result == TRUE) 
            This->Piece->Y++;
        DrawPiece(This);
    } while(Result == TRUE);
}


/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
VOID RotatePiece(BOARD *This)
{
    BODY TmpBody;
    
    CopyMem(&TmpBody, &This->Piece->Body, sizeof(BODY));
    This->Piece->Rotate(&TmpBody);
    ErasePiece(This);
    if (CheckSpace(This, &TmpBody, This->Piece->X, This->Piece->Y) == TRUE) 
        This->Piece->Rotate(&This->Piece->Body);
    DrawPiece(This);
}


/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
BOOLEAN MovePieceLeft(BOARD *This)
{
    BOOLEAN Result = FALSE; 
    if (This->Piece->X) {
        ErasePiece(This);
        Result = CheckSpace(This, &This->Piece->Body, This->Piece->X-1, This->Piece->Y);
        if (Result == TRUE) 
            This->Piece->X--;
        DrawPiece(This);
    }
    return Result;
}

/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
BOOLEAN MovePieceRight(BOARD *This)
{
    BOOLEAN Result = FALSE;  
    if (This->Piece->X < BOARD_WIDTH) {
        ErasePiece(This);
        Result = CheckSpace(This, &This->Piece->Body, This->Piece->X+1, This->Piece->Y);
        if (Result == TRUE) 
            This->Piece->X++;
        DrawPiece(This);
    }
    return Result;
}

/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
BOOLEAN MovePieceDown(BOARD *This)
{
    BOOLEAN Result = FALSE;  
    if (This->Piece->Y < BOARD_HEIGHT) {
        ErasePiece(This);
        Result = CheckSpace(This, &This->Piece->Body, This->Piece->X, This->Piece->Y+1);
        if (Result == TRUE) 
            This->Piece->Y++;
        DrawPiece(This);
    }
    return Result;
}


/**
  @param BOARD *This    pointer to the BOARD stucture.  
**/
BOOLEAN NextPiece(BOARD *This)
{
    This->Piece->New(This->Piece);
    
    // Show a new piece on the screen
    This->Piece->X = BOARD_WIDTH / 2;
    This->Piece->Y = 0;
    return DrawPiece(This);
}



/**
  Draw the game board
**/
VOID ShowGameBoard(VOID)
{
    UINTN  i;
    CHAR16 ChrSide[2] = {0};
    
    gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);
    gST->ConOut->OutputString(gST->ConOut, L" Level : \n\r");
    gST->ConOut->OutputString(gST->ConOut, L" Lines : \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L" Score : \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"\n\n\n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"           HELP  \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"         P  - Pause game \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"        ->  - Left \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"        <-  - Right \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"        Up  - Rotate \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"     Space  - Drop \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"   Page Up  - level up \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L" Page Down  - level down \n\r" );
    gST->ConOut->OutputString(gST->ConOut, L"       ESC  - Exit game \n\r" );

    gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_BLUE, EFI_BLACK));
    
    // bottom 
    ChrSide[0] = BOXDRAW_DOUBLE_HORIZONTAL;
    for (i=BOARD_LTOP_X; i<BOARD_WIDTH; i++) {
        gST->ConOut->SetCursorPosition(gST->ConOut, i, BOARD_LTOP_Y+BOARD_HEIGHT);
        gST->ConOut->OutputString(gST->ConOut, ChrSide);
    }
    
    // side
    ChrSide[0] = BOXDRAW_DOUBLE_VERTICAL;
    for (i=BOARD_LTOP_Y; i<BOARD_HEIGHT; i++) {
        gST->ConOut->SetCursorPosition(gST->ConOut, BOARD_LTOP_X, i);
        gST->ConOut->OutputString(gST->ConOut, ChrSide);
        
        gST->ConOut->SetCursorPosition(gST->ConOut, BOARD_LTOP_X+BOARD_WIDTH, i);
        gST->ConOut->OutputString(gST->ConOut, ChrSide);
    }
    ChrSide[0] = BOXDRAW_DOUBLE_UP_RIGHT;
    gST->ConOut->SetCursorPosition(gST->ConOut, BOARD_LTOP_X, i);
    gST->ConOut->OutputString(gST->ConOut, ChrSide);
    
    ChrSide[0] = BOXDRAW_DOUBLE_UP_LEFT;
    gST->ConOut->SetCursorPosition(gST->ConOut, BOARD_LTOP_X+BOARD_WIDTH, i);
    gST->ConOut->OutputString(gST->ConOut, ChrSide);
}


/**
  This is board constructor. Updates a pointer to the Board. 
  @param BOARD *This    pointer to the BOARD stucture.  
**/
VOID ConstructBoard(BOARD **This)
{
    BOARD *Board = AllocatePool(sizeof(BOARD));
    ZeroMem(Board, sizeof(BOARD));
    
    Board->Height            = BOARD_HEIGHT;
    Board->Width             = BOARD_WIDTH;
    Board->ClearRows         = ClearRows;
    Board->Lines             = 0;
    Board->MovePieceLeft     = MovePieceLeft;
    Board->MovePieceRight    = MovePieceRight;
    Board->MovePieceDown     = MovePieceDown;
    Board->RotatePiece       = RotatePiece;
    Board->DropPiece         = DropPiece;
    Board->ReDrawBoard       = ReDrawBoard;
    Board->NextPiece         = NextPiece;
    
    ConstructPiece(&Board->Piece);
    NextPiece(Board);
    ShowGameBoard();
    *This = Board;
}


/**
  This is board destructor. 
  @param BOARD *This    pointer to the BOARD stucture.  
**/
VOID DestructBoard(BOARD *This)
{
    DestructPiece(This->Piece);
    FreePool(This);
}
