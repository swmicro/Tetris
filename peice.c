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
#include "peice.h"



/**
    Pseudo random generator
    @retval UINT8 pseudo random number
**/
UINT8 Rand8(VOID)
{
    static UINT8 Result = 0;
    
    if (Result == 0) {
        EFI_TIME  Time;
        gRT->GetTime(&Time, NULL);
        Result = Time.Second;
    }
    Result = (Result<<1) | (((Result&0x80)>>7)^((Result&0x40)>>6));
    return Result;
}


/**
    This function rotates the Body clockwise. 
    Xnew = ~y & 3
    Ynew = x
    @param BODY *Body pointer to the Body 
**/
VOID RotateBody(BODY *Body)
{
    UINT8 tmp;
    UINTN i;
    
    for (i=0; i<4; i++) {
        tmp = Body->a[i][0];
        Body->a[i][0] = ~(Body->a[i][1]) & 3;
        Body->a[i][1] = tmp;
    }
}


/**
    Updates the pointer to a new Piece. 
    @param PIECE *Piece pointer to Piece 
**/
VOID NewPiece(PIECE *This)
{
    UINT8 Rand = Rand8();
    BODY  *Body = NULL;
    BODY  Straight = STRAIGHT_BODY;
    BODY  Rectangle = RECTANGLE_BODY;
    BODY  CornerLeft = CORNER_LEFT_BODY;
    BODY  CornerRight = CORNER_RIGHT_BODY;
    BODY  Cross = CROSS_BODY;
    BODY  ZigZag1 = ZIGZAG_1_BODY;
    BODY  ZigZag2 = ZIGZAG_2_BODY;

    This->Color = Rand | 1; // exclude black color

    switch(Rand & 7) {  // the range is 0...7
        case 0:  Body = &Straight;    break;
        case 1:  Body = &Rectangle;   break;
        case 2:  Body = &CornerLeft;  break;
        case 3:  Body = &CornerRight; break;
        case 4:  Body = &Cross;       break;
        case 5:  Body = &ZigZag1;     break;
        case 6:  Body = &ZigZag2;     break;
        default: Body = &Cross;       break;
    }
    CopyMem(&This->Body, Body, sizeof(BODY));
}


/**
    This is piece constructor. 
    @param PIECE *Piece pointer to Piece 
**/
VOID ConstructPiece(PIECE **This)
{
    PIECE *Piece = AllocatePool(sizeof(PIECE));
    ZeroMem(Piece, sizeof(PIECE));
    Piece->New      = NewPiece;
    Piece->Rotate   = RotateBody;
    NewPiece(Piece);
    *This = Piece;
}


/**
    This is piece destructor. 
    @param PIECE *Piece pointer to Piece 
**/
VOID DestructPiece(PIECE *This)
{
    if (This != NULL)
        FreePool(This);
}
