/*									tab:8
 *
 * mp6.h - header file for ECE220 "Blocky" game
 *
 * "Copyright (c) 2017-2018 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steve Lumetta
 * Version:	    1
 * Creation Date:   30 November 2017
 * Filename:	    mp6.h
 * History:
 *	SL	1	30 November 2017
 *		First written.
 *	SL	2	20 February 2018
 *		Cleaned up for distribution in ECE220H S18.
 */

#if !defined(MP6_H)
#define MP6_H

#include <stdint.h>


/************************************************************************
 * PREPROCESSOR CONSTANTS                                               *
 ************************************************************************/

/* 
 * Change to 1 to play the game AFTER you have debugged in text mode.
 */
#define USE_NCURSES 0

/* 
 * These constants define the board width and height.  Feel free to
 * change them within reason.  We may test your code with various sizes.
 */
#define BOARD_WIDTH  11
#define BOARD_HEIGHT 15


/************************************************************************
 * TYPE DEFINITIONS                                                     *
 ************************************************************************/

/*
 * A board is a 2D array of spaces, each of which can be empty, filled
 * with the current piece, or filled with a block from an old (and 
 * stationary) piece.
 */
typedef enum {
    SPACE_EMPTY,	/* empty         */
    SPACE_FULL,		/* old piece     */
    SPACE_BLOCK,	/* current piece */
    NUM_SPACE_TYPES
} space_type_t;

/* Each piece has a type. */
typedef enum {
    PIECE_TYPE_ELL,	/* an 'L'                 */
    PIECE_TYPE_REV_ELL, /* mirror image of an 'L' */
    PIECE_TYPE_LINE,    /* a straight line        */
    PIECE_TYPE_TEE,     /* a 'T'                  */
    PIECE_TYPE_ESS,     /* an 'S'                 */
    PIECE_TYPE_REV_ESS, /* mirror image of an 'S' */
    NUM_PIECE_TYPES
} piece_type_t;

/*
 * This type is used to return results when checking whether a piece
 * fits into the board at a given location.
 */
typedef enum {
    FIT_SUCCESS,	/* piece fits as specified                   */
    FIT_OUT_OF_BOARD,	/* piece not within bounds of board          */
    FIT_NO_ROOM_THERE	/* spaces under the piece's blocks not empty */
} fit_result_t;


/************************************************************************
 * YOU MUST IMPLEMENT THE ROUTINES IN THE NEXT SECTION IN mp2.c.        *
 ************************************************************************/

/* 
 * Fills the board b with empty spaces.  Returns 1 on success, 
 * or 0 on failure. 
 */
extern int32_t empty_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH]);

/* 
 * Forcibly writes a piece into a board, ignoring the board's current
 * contents.
 */
extern void mark_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
			piece_type_t p, int32_t orient, 
			int32_t x, int32_t y, space_type_t v);

/* 
 * Checks whether a piece fits into a board.  If the piece does not fit
 * within the board, returns FIT_OUT_OF_BOARD.  Otherwise, if the piece
 * would overwrite non-empty board spaces, returns FIT_NO_ROOM_THERE.
 * Otherwise, returns FIT_SUCCESS.
 */
extern fit_result_t test_piece_fit 
	(space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], piece_type_t p, 
	 int32_t orient, int32_t x, int32_t y);

/* 
 * Prints a board to the screen using period ('.') to represent empty
 * spaces, percent ('%') to represent old blocks, and asterisk ('*')
 * to represent the current piece (SPACE_BLOCK).
 */
extern void print_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH]);

/*
 * Tries to move the current piece down one space in the board.  On
 * success, returns 1.  Otherwise, returns 0.
 */
extern int32_t try_to_move_down 
	(space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], piece_type_t p, 
	 int32_t orient, int32_t x, int32_t y);

/*
 * Tries to move the current piece left one space in the board.  On
 * success, returns 1.  Otherwise, returns 0.
 */
extern int32_t try_to_move_left 
	(space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], piece_type_t p, 
	 int32_t orient, int32_t x, int32_t y);

/*
 * Tries to move the current piece right one space in the board.  On
 * success, returns 1.  Otherwise, returns 0.
 */
extern int32_t try_to_move_right 
	(space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], piece_type_t p, 
	 int32_t orient, int32_t x, int32_t y);

/*
 * Tries to rotate the current piece clockwise in the board.  On success, 
 * returns 1.  Otherwise, returns 0.
 */
extern int32_t try_to_rotate_clockwise 
	(space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], piece_type_t p, 
	 int32_t orient, int32_t x, int32_t y);

/*
 * Tries to rotate the current piece counter-clockwise in the board.  
 * On success, returns 1.  Otherwise, returns 0.
 */
extern int32_t try_to_rotate_cc 
	(space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], piece_type_t p, 
	 int32_t orient, int32_t x, int32_t y);

/* 
 * Removes row with index 'row' from a board, moving rows above it 
 * downward and adding a new empty row at the top of the board.
 */
extern void remove_row (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
			int row);

/*
 * Removes all rows filled with SPACE_FULL from the board, moving
 * rows above them downward and adding new empty rows at the top of
 * the board.
 */
extern void remove_full_rows (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH]);


/************************************************************************
 * THE ROUTINES IN THIS SECTION ARE PROVIDED FOR YOUR USE (in main.c).  *
 ************************************************************************/

/*
 * Place a piece of type p in a specific orientation onto the board b
 * at location (x,y) by marking the piece's blocks with SPACE_BLOCK. 
 */
extern void add_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
		       piece_type_t p, int32_t orient, int32_t x, int32_t y);

/* 
 * Remove a piece of type p in a specific orientation from the board b
 * at location (x,y), overwriting the piece with SPACE_EMPTY.
 */
extern void remove_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
		          piece_type_t p, int32_t orient, 
			  int32_t x, int32_t y);


/************************************************************************
 * THESE PREPROCESSOR MACROS IMPLEMENT THE DIFFERENCES BETWEEN TEXT AND *
 * 'GRAPHICS' MODES.                                                    *
 ************************************************************************/

#if USE_NCURSES
#include "ncurses.h"
#define printf printw
#else /* !USE_N_CURSES */
/* Eliminate some ncurses library calls. */
#define clear()
#endif /* differences with USE_N_CURSES */

#endif /* MP6_H */

