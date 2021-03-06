// File: foptimove.h
// Provides: class FOptiMove
//
//  Standalone class
//  ════════════════
//
// ▕▔▔▔▔▔▔▔▔▔▔▔▏
// ▕ FOptiMove ▏
// ▕▁▁▁▁▁▁▁▁▁▁▁▏

// The cursor optimization based on ncurses lib_mvcur.c
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _FOPTIMOVE_H
#define _FOPTIMOVE_H

#include <assert.h>
#include <term.h>  // need for tparm

#include <cctype>
#include <climits>
#include <cstdio>  // need for printf
#include <cstdlib>
#include <cstring>


//----------------------------------------------------------------------
// class FOptiMove
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FOptiMove
{
 public:
   // Constructor
   explicit FOptiMove (int = 0);

   // Destructor
  ~FOptiMove();

   // Mutators
   void  setBaudRate (int);
   void  setTabStop (int);
   void  setTermSize (int, int);
   int   set_cursor_home (char*&);
   int   set_cursor_to_ll (char*&);
   int   set_carriage_return (char*&);
   int   set_tabular (char*&);
   int   set_back_tab (char*&);
   int   set_cursor_up (char*&);
   int   set_cursor_down (char*&);
   int   set_cursor_left (char*&);
   int   set_cursor_right (char*&);
   int   set_cursor_address (char*&);
   int   set_column_address (char*&);
   int   set_row_address (char*&);
   int   set_parm_up_cursor (char*&);
   int   set_parm_down_cursor (char*&);
   int   set_parm_left_cursor (char*&);
   int   set_parm_right_cursor (char*&);
   int   set_erase_chars (char*&);
   int   set_repeat_char (char*&);
   int   set_clr_bol (char*&);
   int   set_clr_eol (char*&);
   void  set_auto_left_margin (bool&);
   void  set_eat_newline_glitch (bool&);

   // Methods
   char* moveCursor (int, int, int, int);
   void  printDurations();

 private:
   // Typedefs
   typedef unsigned char uChar;
   typedef unsigned int  uInt;

   typedef struct
   {
     char* cap;
     int duration;
     int length;
   } capability;

   // Constants
   static const int LONG_DURATION = INT_MAX;
   // value for a long capability waiting time
   static const int MOVE_LIMIT = 7;
   // maximum character distance to avoid direct cursor addressing

   // Methods
   void calculateCharDuration();
   int  capDuration (char*&, int);
   int  capDurationToLength (int);
   int  repeatedAppend (capability&, int, char*);
   int  relativeMove (char*&, int, int, int, int);
   bool isWideMove (int, int, int, int);

   // Data Members
   capability F_cursor_home;
   capability F_carriage_return;
   capability F_cursor_to_ll;
   capability F_tab;
   capability F_back_tab;
   capability F_cursor_up;
   capability F_cursor_down;
   capability F_cursor_left;
   capability F_cursor_right;
   capability F_cursor_address;
   capability F_column_address;
   capability F_row_address;
   capability F_parm_up_cursor;
   capability F_parm_down_cursor;
   capability F_parm_left_cursor;
   capability F_parm_right_cursor;
   capability F_erase_chars;
   capability F_repeat_char;
   capability F_clr_bol;
   capability F_clr_eol;

   bool       automatic_left_margin;
   bool       eat_nl_glitch;

   char       move_buf[512];
   int        char_duration;
   int        baudrate;
   int        tabstop;
   int        screen_width;
   int        screen_height;
};
#pragma pack(pop)


// FOptiMove inline functions
//----------------------------------------------------------------------
inline void FOptiMove::set_auto_left_margin (bool& bcap)
{ automatic_left_margin = bcap; }

//----------------------------------------------------------------------
inline void FOptiMove::set_eat_newline_glitch (bool& bcap)
{ eat_nl_glitch = bcap; }

#endif  // _FOPTIMOVE_H
