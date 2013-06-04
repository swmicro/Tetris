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

#ifndef __PIECE_H
#define __PIECE_H

#include <Uefi.h>


//------------------------------------------------------------------------------
// The Peice
//

// OOOO
#define STRAIGHT_BODY       {1,0, 1,1, 1,2, 1,3}

//  OO
//  OO
#define RECTANGLE_BODY      {1,0, 1,1, 2,0, 2,1}

// O
// OOO
#define CORNER_LEFT_BODY    {0,0, 0,1, 1,0, 2,0}

//   O
// OOO
#define CORNER_RIGHT_BODY   {0,0, 1,0, 2,0, 2,1}

//  O
// OOO
#define CROSS_BODY          {0,0, 1,0, 1,1, 2,0}

// OO
//  OO
#define ZIGZAG_1_BODY       {0,0, 1,0, 1,1, 2,1}

//  OO
// OO
#define ZIGZAG_2_BODY       {0,1, 1,0, 1,1, 2,0}


typedef struct _BODY {
    UINT8   a[4][2];
} BODY;

typedef struct _PIECE PIECE;
typedef VOID (*ROTATE)(BODY *Body);
typedef VOID (*NEW_PIECE)(PIECE *This);

typedef struct _PIECE {
    BODY        Body;
    UINT8       Color;
    UINT8       X;
    UINT8       Y;
    ROTATE      Rotate;
    NEW_PIECE   New;
} PIECE;

VOID ConstructPiece(PIECE **This);
VOID DestructPiece(PIECE *This);

#endif
