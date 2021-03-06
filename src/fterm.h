// File: fterm.h
// Provides: class FTerm
//
//  Base class
//  ══════════
//
// ▕▔▔▔▔▔▔▔▏ 1      1▕▔▔▔▔▔▔▔▔▔▔▔▏
// ▕ FTerm ▏-┬- - - -▕ FOptiMove ▏
// ▕▁▁▁▁▁▁▁▏ :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
//           :
//           :      1▕▔▔▔▔▔▔▔▔▔▔▔▏
//           :- - - -▕ FOptiAttr ▏
//           :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
//           :
//           :      *▕▔▔▔▔▔▔▔▔▔▏
//           :- - - -▕ FString ▏
//           :       ▕▁▁▁▁▁▁▁▁▁▏
//           :
//           :      *▕▔▔▔▔▔▔▔▔▏
//           :- - - -▕ FPoint ▏
//           :       ▕▁▁▁▁▁▁▁▁▏
//           :
//           :      *▕▔▔▔▔▔▔▔▏
//           └- - - -▕ FRect ▏
//                   ▕▁▁▁▁▁▁▁▏

#ifndef _FTERM_H
#define _FTERM_H

#include <fconfig.h>

#ifdef F_HAVE_LIBGPM
  #include <gpm.h>
#endif

#include <linux/fb.h>       // Linux framebuffer console
#include <linux/keyboard.h> // need for gpm keyboard modifiers

#include <sys/io.h>         // <asm/io.h> is deprecated, use <sys/io.h> instead
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <langinfo.h>
#include <term.h>           // termcap
#include <termios.h>
#include <unistd.h>

#include <cmath>
#include <csignal>
#include <map>
#include <queue>

#include "fc.h"
#include "fobject.h"
#include "foptiattr.h"
#include "foptimove.h"
#include "fpoint.h"
#include "frect.h"
#include "fstring.h"
#include "ftermcap.h"


#ifdef F_HAVE_LIBGPM
  #undef buttons  // from term.h
#endif

// ascii sequences
#define ENQ    "\005"     // Enquiry
#define BEL    "\007"     // Bell (ctrl‐g)
#define BS     "\010"     // Backspace
#define SO     "\016"     // Shift out (alternative character set)
#define SI     "\017"     // Shift in  (regular character set)
#define OSC    ESC "]"    // Operating system command (7-bit)
#define SECDA  ESC "[>c"  // Secondary Device Attributes


//----------------------------------------------------------------------
// class FTerm
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FTerm
{
 public:
   // Typedefs
   typedef FOptiAttr::char_data  char_data;

   static struct modifier_key // bit field
   {
     uChar shift  : 1;  // 0..1
     uChar alt_gr : 1;  // 0..1
     uChar ctrl   : 1;  // 0..1
     uChar alt    : 1;  // 0..1
     uChar        : 4;  // padding bits
   } mod_key;

   // Constructor
   FTerm ();

   // Destructor
   virtual ~FTerm();

   // Accessors
   virtual const char*  getClassName() const;
   static termios       getTTY();
   static int           getLineNumber();
   static int           getColumnNumber();
   static FString       getKeyName (int);
   static modifier_key& getModifierKey();
   static char*         getTermType();
   static char*         getTermName();
   static uInt          getTabstop();
   static int           getMaxColor();
   static fc::consoleCursorStyle getConsoleCursor();

   // Inquiries
   static bool          isKeyTimeout (timeval*, register long);
   static bool          isNormal (char_data*&);
   static bool          isRaw();
   static bool          hasPCcharset();
   static bool          hasUTF8();
   static bool          hasVT100();
   static bool          hasASCII();
   static bool          isMonochron();
   static bool          isXTerminal();
   static bool          isAnsiTerminal();
   static bool          isRxvtTerminal();
   static bool          isUrxvtTerminal();
   static bool          isMltermTerminal();
   static bool          isPuttyTerminal();
   static bool          isKdeTerminal();
   static bool          isGnomeTerminal();
   static bool          isKtermTerminal();
   static bool          isTeraTerm();
   static bool          isCygwinTerminal();
   static bool          isMinttyTerm();
   static bool          isLinuxTerm();
   static bool          isScreenTerm();
   static bool          isTmuxTerm();
   static bool          isInputDataPending();
   static bool          isNewFont();
   static bool          isUTF8();
   static bool          isUTF8_linux_terminal();

   // Mutators
   static bool          setCursorOptimisation (bool);
   static void          setXTermDefaultColors (bool);
   static void          setConsoleCursor (fc::consoleCursorStyle, bool);
   static void          setTTY (termios&);
   static void          noHardwareEcho();
   static bool          setRawMode (bool);
   static bool          setRawMode();
   static bool          unsetRawMode();
   static bool          setCookedMode();
   static bool          setUTF8 (bool);
   static bool          setUTF8();
   static bool          unsetUTF8();
   static bool          setNonBlockingInput (bool);
   static bool          setNonBlockingInput();
   static bool          unsetNonBlockingInput();

   // Methods
   static int           parseKeyString (char[], int, timeval*);
   static bool&         unprocessedInput();
   static bool          setVGAFont();
   static bool          setNewFont();
   static bool          setOldFont();
   static char*         moveCursor (int, int, int, int);
   static void          printMoveDurations();
   static char*         enableCursor();
   static char*         disableCursor();
   static void          detectTermSize();
   static void          setTermSize (int, int);
   static void          setKDECursor (fc::kdeKonsoleCursorShape);
   static FString*      getXTermFont();
   static FString*      getXTermTitle();
   static const FString getXTermColorName (int);
   static void          setXTermCursorStyle (fc::xtermCursorStyle);
   static void          setXTermTitle (const FString&);
   static void          setXTermForeground (const FString&);
   static void          setXTermBackground (const FString&);
   static void          setXTermCursorColor (const FString&);
   static void          setXTermMouseForeground (const FString&);
   static void          setXTermMouseBackground (const FString&);
   static void          setXTermHighlightBackground (const FString&);
   static void          setXTermDefaults();
   static void          resetXTermColors();
   static void          resetXTermForeground();
   static void          resetXTermBackground();
   static void          resetXTermCursorColor();
   static void          resetXTermMouseForeground();
   static void          resetXTermMouseBackground();
   static void          resetXTermHighlightBackground();
   static void          resetXTermDefaults();
   static void          saveColorMap();
   static void          resetColorMap();
   static void          setPalette (short, int, int, int);
   static void          setBeep (int, int);
   static void          resetBeep();
   static void          beep();

   static void          setEncoding (std::string);
   static std::string   getEncoding();

   static bool          scrollTermForward();
   static bool          scrollTermReverse();

   static const FString getAnswerbackMsg();
   static const FString getSecDA();

   // function pointer -> static function
   static int           (*Fputchar)(int);

   static void          putstringf (const char*, ...)
   #if defined(__clang__)
     __attribute__((__format__ (__printf__, 1, 2)))
   #elif defined(__GNUC__)
     __attribute__ ((format (printf, 1, 2)))
   #endif
                        ;
   static void          putstring (const char*, int = 1);
   static int           putchar_ASCII (register int);
   static int           putchar_UTF8  (register int);
   static int           UTF8decode (const char[]);

 protected:
   // Methods
   static void          init_consoleCharMap();
   static bool          charEncodable (uInt);
   static uInt          charEncode (uInt);
   static uInt          charEncode (uInt, fc::encoding);
   static char*         changeAttribute ( char_data*&
                                        , char_data*& );
   static bool          hasChangedTermSize();
   static void          changeTermSizeFinished();
   static void          xtermMouse (bool);
   static void          enableXTermMouse();
   static void          disableXTermMouse();

#ifdef F_HAVE_LIBGPM
   static bool          gpmMouse (bool);
   static bool          enableGpmMouse();
   static bool          disableGpmMouse();
   static bool          isGpmMouseEnabled();
#endif  // F_HAVE_LIBGPM
   static FPoint&       getMousePos();
   static void          setMousePos (FPoint&);
   static void          setMousePos (short, short);

   // Data Members
   static int           stdin_no;
   static int           stdout_no;
   static int           erase_ch_length;
   static int           repeat_char_length;
   static int           clr_bol_length;
   static int           clr_eol_length;
   static int           cursor_addres_lengths;
   static bool          NewFont;
   static bool          VGAFont;
   static bool          no_shadow_character;
   static bool          no_half_block_character;
   static bool          cursor_optimisation;
   static bool          xterm_default_colors;
   static fc::encoding  Encoding;
   static char          exit_message[8192];

 private:
   // Typedefs
   typedef FTermcap::tcap_map termcap_map;

   typedef struct
   {
     uChar red;
     uChar green;
     uChar blue;
   } dacreg;

   // Constants
   static const int NEED_MORE_DATA = -1;  // parseKeyString return value

   // Disable copy constructor
   FTerm (const FTerm&);
   // Disable assignment operator (=)
   FTerm& operator = (const FTerm&);

   // Inquiries
   static int           isConsole();

   // Methods
   static uInt16        getInputStatusRegisterOne();
   static uChar         readAttributeController (uChar);
   static void          writeAttributeController (uChar, uChar);
   static uChar         getAttributeMode();
   static void          setAttributeMode (uChar);
   static int           setBlinkAsIntensity (bool);
   static int           getFramebuffer_bpp();
   static int           openConsole();
   static int           closeConsole();
   static void          identifyTermType();
   static void          storeTTYsettings();
   static void          restoreTTYsettings();
   static int           getScreenFont();
   static int           setScreenFont (uChar*, uInt, uInt, uInt, bool = false);
   static int           setUnicodeMap (struct unimapdesc*);
   static int           getUnicodeMap ();
   static void          init_console();
   static uInt          getBaudRate (const struct termios*);
   static char*         init_256colorTerminal();
   static char*         parseAnswerbackMsg (char*&);
   static char*         parseSecDA (char*&);
   static void          oscPrefix();
   static void          oscPostfix();
   static void          init_alt_charset();
   static void          init_pc_charset();
   static void          init_termcaps();
   static void          init_encoding();
   void                 init();
   void                 finish();
   static uInt          cp437_to_unicode (uChar);
   static void          signal_handler (int);

   // Data Members
   static std::map <uChar,uChar>* vt100_alt_char;
   static std::map <std::string,fc::encoding>* encoding_set;
   static FTermcap::tcap_map* tcap;

   static bool    mouse_support;
   static bool    terminal_detection;
   static bool    raw_mode;
   static bool    input_data_pending;
   static bool    non_blocking_stdin;
   static bool    gpm_mouse_enabled;
   static bool    pc_charset_console;
   static bool    utf8_input;
   static bool    utf8_state;
   static bool    utf8_console;
   static bool    utf8_linux_terminal;
   static bool    force_vt100;
   static bool    vt100_console;
   static bool    ascii_console;
   static bool    color256;
   static bool    monochron;
   static bool    xterm_terminal;
   static bool    ansi_terminal;
   static bool    rxvt_terminal;
   static bool    urxvt_terminal;
   static bool    mlterm_terminal;
   static bool    putty_terminal;
   static bool    kde_konsole;
   static bool    gnome_terminal;
   static bool    kterm_terminal;
   static bool    tera_terminal;
   static bool    cygwin_terminal;
   static bool    mintty_terminal;
   static bool    linux_terminal;
   static bool    screen_terminal;
   static bool    tmux_terminal;
   static char    termtype[30];
   static char*   term_name;
   static char*   locale_name;
   static char*   locale_xterm;
   static FRect*  term;      // current terminal geometry
   static FPoint* mouse;     // mouse click position

   static int     stdin_status_flags;
   static int     fd_tty;
   static uInt    baudrate;
   static bool    resize_term;

   static struct  termios term_init;

   static fc::consoleCursorStyle console_cursor_style;
   static struct console_font_op screen_font;
   static struct unimapdesc      screen_unicode_map;

   static FOptiMove*     opti_move;
   static FOptiAttr*     opti_attr;
   static const FString* xterm_font;
   static const FString* xterm_title;
   static const FString* answer_back;
   static const FString* sec_da;

   struct
   {
     dacreg d[16];
   } color_map;
};

#pragma pack(pop)

// FTerm inline functions
//----------------------------------------------------------------------
inline const char* FTerm::getClassName() const
{ return "FTerm"; }

//----------------------------------------------------------------------
inline char* FTerm::getTermType()
{ return termtype; }

//----------------------------------------------------------------------
inline char* FTerm::getTermName()
{ return term_name; }

//----------------------------------------------------------------------
inline uInt FTerm::getTabstop()
{ return FTermcap::tabstop; }

//----------------------------------------------------------------------
inline int FTerm::getMaxColor()
{ return FTermcap::max_color; }

//----------------------------------------------------------------------
inline bool FTerm::isRaw()
{ return raw_mode; }

//----------------------------------------------------------------------
inline bool FTerm::hasPCcharset()
{ return pc_charset_console; }

//----------------------------------------------------------------------
inline bool FTerm::hasUTF8()
{ return utf8_console; }

//----------------------------------------------------------------------
inline bool FTerm::hasVT100()
{ return vt100_console; }

//----------------------------------------------------------------------
inline bool FTerm::hasASCII()
{ return ascii_console; }

//----------------------------------------------------------------------
inline bool FTerm::isMonochron()
{ return monochron; }

//----------------------------------------------------------------------
inline bool FTerm::isXTerminal()
{ return xterm_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isAnsiTerminal()
{ return ansi_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isRxvtTerminal()
{ return rxvt_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isUrxvtTerminal()
{ return urxvt_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isMltermTerminal()
{ return mlterm_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isPuttyTerminal()
{ return putty_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isKdeTerminal()
{ return kde_konsole; }

//----------------------------------------------------------------------
inline bool FTerm::isGnomeTerminal()
{ return gnome_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isKtermTerminal()
{ return kterm_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isTeraTerm()
{ return tera_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isCygwinTerminal()
{ return cygwin_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isMinttyTerm()
{ return mintty_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isLinuxTerm()
{ return linux_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isScreenTerm()
{ return screen_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isTmuxTerm()
{ return tmux_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::isInputDataPending()
{ return input_data_pending; }

//----------------------------------------------------------------------
inline bool FTerm::isNewFont()
{ return NewFont; }

//----------------------------------------------------------------------
inline bool FTerm::isUTF8()
{ return utf8_state; }

//----------------------------------------------------------------------
inline bool FTerm::isUTF8_linux_terminal()
{ return utf8_linux_terminal; }

//----------------------------------------------------------------------
inline bool FTerm::setCursorOptimisation (bool on)
{ return cursor_optimisation = (on) ? true : false; }

//----------------------------------------------------------------------
inline void FTerm::setXTermDefaultColors (bool on)
{ xterm_default_colors = on; }

//----------------------------------------------------------------------
inline bool FTerm::setRawMode()
{ return setRawMode(true); }

//----------------------------------------------------------------------
inline bool FTerm::unsetRawMode()
{ return setRawMode(false); }

//----------------------------------------------------------------------
inline bool FTerm::setCookedMode()
{ return setRawMode(false); }

//----------------------------------------------------------------------
inline bool FTerm::setUTF8()
{ return setUTF8(true); }

//----------------------------------------------------------------------
inline bool FTerm::unsetUTF8()
{ return setUTF8(false); }

//----------------------------------------------------------------------
inline bool FTerm::setNonBlockingInput()
{ return setNonBlockingInput(true); }

//----------------------------------------------------------------------
inline bool FTerm::unsetNonBlockingInput()
{ return setNonBlockingInput(false); }

//----------------------------------------------------------------------
inline bool FTerm::hasChangedTermSize()
{ return resize_term; }

//----------------------------------------------------------------------
inline void FTerm::changeTermSizeFinished()
{ resize_term = false; }

//----------------------------------------------------------------------
inline void FTerm::enableXTermMouse()
{ xtermMouse(true); }

//----------------------------------------------------------------------
inline void FTerm::disableXTermMouse()
{ xtermMouse(false); }

#ifdef F_HAVE_LIBGPM
//----------------------------------------------------------------------
inline bool FTerm::enableGpmMouse()
{ return gpmMouse(true); }

//----------------------------------------------------------------------
inline bool FTerm::disableGpmMouse()
{ return gpmMouse(false); }

//----------------------------------------------------------------------
inline bool FTerm::isGpmMouseEnabled()
{ return gpm_mouse_enabled; }
#endif  // F_HAVE_LIBGPM

//----------------------------------------------------------------------
inline FPoint& FTerm::getMousePos()
{ return *mouse; }

//----------------------------------------------------------------------
inline void FTerm::setMousePos (FPoint& m)
{ *mouse = m; }

//----------------------------------------------------------------------
inline void FTerm::setMousePos (short x, short y)
{ mouse->setPoint (x, y); }


#endif  // _FTERM_H
