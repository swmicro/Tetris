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

#ifndef __GAME_H
#define __GAME_H

#include <Uefi.h>
#include "board.h"


//------------------------------------------------------------------------------
// The Game
//


// periodic timer initializing constant (1mS = 10000)
#define TIMER_PERIOD_INIT       100000
#define GAME_REFRESH_PERIOD     100

enum {
    MENU_GAME = 1,
    PLAY_GAME,
    GAME_OVER,
    EXIT_GAME
} GAME_STATUS;

typedef struct _GAME GAME;
typedef VOID (*PARSE_KEYPRESS)(GAME *This);
typedef VOID (*REDRAW)(GAME *This);

typedef struct _GAME {
    BOARD           *Board;
    BOOLEAN         PauseGame;
    UINTN           Status;
    UINTN           GameTimer;
    UINTN           Level;
    UINTN           Lines;
    UINTN           Score;
    EFI_EVENT       TimerEvent;
    PARSE_KEYPRESS  ParseKeyPress;
    REDRAW          ReDraw;
} GAME;

VOID ConstructGame(GAME **This);
VOID DestructGame(GAME *This);

#endif
