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

#ifndef __BOARD_H
#define __BOARD_H

#include <Uefi.h>
#include "piece.h"


//------------------------------------------------------------------------------
// The Board
//

#define BOARD_HEIGHT            19
#define BOARD_WIDTH             20
#define BOARD_LTOP_X            30
#define BOARD_LTOP_Y            0


typedef struct _BOARD BOARD;
typedef VOID (*CLEAR_ROWS)(BOARD *This);
typedef BOOLEAN (*MOVE_PIECE_LEFT)(BOARD *This);
typedef BOOLEAN (*MOVE_PIECE_RIGHT)(BOARD *This);
typedef BOOLEAN (*MOVE_PIECE_DOWN)(BOARD *This);
typedef VOID (*ROTATE_PIECE)(BOARD *This);
typedef VOID (*DROP_PIECE)(BOARD *This);
typedef VOID (*REDRAW_BOARD)(BOARD *This);
typedef BOOLEAN (*NEXT_PIECE)(BOARD *This);


typedef struct _BOARD {
    UINT8               Field[BOARD_HEIGHT][BOARD_WIDTH];
    UINT8               Height;
    UINT8               Width;
    UINTN               Lines;
    CLEAR_ROWS          ClearRows;
    REDRAW_BOARD        ReDrawBoard;
    PIECE               *Piece;
    MOVE_PIECE_LEFT     MovePieceLeft;
    MOVE_PIECE_RIGHT    MovePieceRight;
    MOVE_PIECE_DOWN     MovePieceDown;
    ROTATE_PIECE        RotatePiece;
    DROP_PIECE          DropPiece;
    NEXT_PIECE          NextPiece;
} BOARD;

VOID ConstructBoard(BOARD **This);
VOID DestructBoard(BOARD *This);

#endif
