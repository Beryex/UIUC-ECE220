/*									tab:8
 *
 * main.c - main program for ECE220 "Blocky" game
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
 * Version:	    5
 * Creation Date:   7 November 2017
 * Filename:	    main.c
 * History:
 *	SL	1	7 November 2017
 *		First written.
 *      JXG     2	8 November 2017
 *		Added ncurses support.
 *	SL	3	30 November 2017
 *		Polished for MP distribution.
 *	SL	4	20 February 2018
 *		Edited slightly for ECE220H S18 distribution.
 */

#include <stdint.h>
#include <stdio.h>

#include "mp6.h"

#define TERMINATE_NOW 10000000 /* Used to terminate game (not a character). */


/* 
 * get_new_piece -- generate a new piece type using a (bad) pseudo-random
 *                  generator
 * INPUTS: none
 * OUTPUTS: none
 * RETURN VALUE: the new piece type
 * SIDE EFFECTS: advances the seed for the random number generator
 */
static piece_type_t
get_new_piece ()
{
    static uint32_t seed = 0xABCDFF97;

    /* 
     * a bad pseudo-random sequence with a fixed seed; students do
     * not want random behavior when debugging
     */
    seed = seed * 137 + 17;
    return (seed % NUM_PIECE_TYPES);
}


/* 
 * add_piece -- place a piece onto a board by marking the piece's blocks
 *		with SPACE_BLOCK; intended for use by the student's 
 *		try_to_* routines
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 * OUTPUTS: the board, with the piece added
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
add_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
           piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    mark_piece (b, p, orient, x, y, SPACE_BLOCK);
}


/* 
 * remove_piece -- remove a piece from a board by marking the piece's 
 *		   blocks with SPACE_EMPTY; intended for use by the 
 *		   student's try_to_* routines
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 * OUTPUTS: the board, with the piece removed
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
remove_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
              piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    mark_piece (b, p, orient, x, y, SPACE_EMPTY);
}


/* 
 * finish_piece -- finish moving a piece in a board by marking the piece's 
 *		   blocks with SPACE_FULL; used only within main.c only
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 * OUTPUTS: the board, with the piece removed
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
static void
finish_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
              piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    mark_piece (b, p, orient, x, y, SPACE_FULL);
}


#if USE_NCURSES

static void
launch_ncurses ()
{
    initscr ();
    noecho ();
    raw ();
    refresh ();
}

static void
stop_ncurses ()
{
    printw ("Press any key to quit.\n");
    refresh ();
    /* Flush input queue. */
    while (ERR != getch ()) { }
    halfdelay (100);
    while (ERR == getch ()) { }
    endwin ();
}

static void
clear_and_print_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    clear ();
    print_board (b);
}

static int
read_char ()
{
    int in;
    
    halfdelay (5);
    if (ERR == (in = getch ())) {
        in = 's'; /* move down when no key is pressed */
    }
    return in;
}

#else /* !USE_NCURSES */

/* These routines do nothing. */
static void launch_ncurses () { }
static void stop_ncurses () { }

static void
clear_and_print_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    printf ("\n");
    print_board (b);
}

static int32_t
read_char ()
{
    char buf[100];

    printf ("\nEnter command: ");
    if (NULL == fgets (buf, 100, stdin)) {
        return TERMINATE_NOW;
    }
    return buf[0];
}

#endif

int
main ()
{
    space_type_t board[BOARD_HEIGHT][BOARD_WIDTH];
    piece_type_t p;
    int32_t orient;
    int32_t x;
    int32_t y;
    int32_t in;

    if (1 != empty_board (board)) {
        fputs ("could not empty board.\n", stderr);
	return 3;
    }

    /* Set up for ncurses, if necessary. */
    launch_ncurses ();

    /* new piece loop */
    while (1) {
        p = get_new_piece ();
	orient = 0;
	x = BOARD_WIDTH / 2;
	y = 0;
	while (FIT_OUT_OF_BOARD == test_piece_fit (board, p, orient, x, y)) {
	    y++;
	}

	if (FIT_NO_ROOM_THERE == test_piece_fit (board, p, orient, x, y)) {
	    /* game over */
	    clear_and_print_board (board);
	    printf ("\n\nGood game.\n");
	    stop_ncurses ();
	    return 0;
	}

	add_piece (board, p, orient, x, y);

	/* command loop */
	while (1) {
	    clear_and_print_board (board);

	    printf ("\nControls:\n\n");
	    printf ("       w: rotate     e: rotate\n");
	    printf ("          counter-      clockwise\n");
	    printf ("          clockwise\n\n");
	    printf ("a: left       s: down       d: right\n");

	    if (TERMINATE_NOW == (in = read_char ())) {
	        printf ("\n\nGame ended.\n");
		return 0;
	    }

	    switch (in) {
		case 'w':
		    if (try_to_rotate_cc (board, p, orient, x, y)) {
		        orient = (orient + 3) % 4;
		    }
		    continue;
		case 'e':
		    if (try_to_rotate_clockwise (board, p, orient, x, y)) {
		        orient = (orient + 1) % 4;
		    }
		    continue;
		case 'a':
		    if (try_to_move_left (board, p, orient, x, y)) {
		        x--;
		    }
		    continue;
	        case 's':
		    if (try_to_move_down (board, p, orient, x, y)) {
		        y++;
			continue;
		    }
		    /* 
		     * cannot move down--lock piece in place and get
		     * new piece 
		     */
		    break;
		case 'd':
		    if (try_to_move_right (board, p, orient, x, y)) {
		        x++;
		    }
		    continue;
		default:
		    printf ("\nBad command.  Try again.\n\n");
		    continue;
	    }
	    finish_piece (board, p, orient, x, y);

	    remove_full_rows (board);

	    break;
	}
    }
}

