// File: fvterm.h
// Provides: class FVTerm
//
//  Inheritance diagram
//  ═══════════════════
//
// ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
// ▕ FObject ▏ ▕  FTerm  ▏
// ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
//      ▲           ▲
//      │           │
//      └─────┬─────┘
//            │
//        ▕▔▔▔▔▔▔▔▔▏ 1      *▕▔▔▔▔▔▔▔▔▔▏
//        ▕ FVTerm ▏-┬- - - -▕ FString ▏
//        ▕▁▁▁▁▁▁▁▁▏ :       ▕▁▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▔▏
//                   :- - - -▕ FPoint ▏
//                   :       ▕▁▁▁▁▁▁▁▁▏
//                   :
//                   :      *▕▔▔▔▔▔▔▔▏
//                   └- - - -▕ FRect ▏
//                           ▕▁▁▁▁▁▁▁▏

#ifndef _FVTERM_H
#define _FVTERM_H

#include "fterm.h"
#include <sstream>  // std::stringstream

// Preprocessing handler macro
#define _PREPROC_HANDLER(i,h) \
           reinterpret_cast<FVTerm*>((i)) \
         , reinterpret_cast<FVTerm::FPreprocessingHandler>((h))

// class forward declaration
class FWidget;

//----------------------------------------------------------------------
// class FVTerm
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FVTerm : public FObject, public FTerm
{
 public:
   // Typedefs and Enumeration
   typedef struct
   {
     uInt xmin;           // X-position with the first change
     uInt xmax;           // X-position with the last change
     uInt trans_count;    // Number of transparent characters
   } line_changes;

   typedef FOptiAttr::char_data  char_data;
   typedef void (FVTerm::*FPreprocessingHandler)();

   typedef struct
   {
     int offset_top;      // Distance from top of the terminal
     int offset_left;     // Distance from left terminal side
     int width;           // Window width
     int height;          // Window height
     int right_shadow;    // Right window shadow
     int bottom_shadow;   // Bottom window shadow
     int cursor_x;        // X-position for the next write operation
     int cursor_y;        // Y-position for the next write operation
     int input_cursor_x;  // X-position input cursor
     int input_cursor_y;  // Y-position input cursor
     FWidget* widget;     // Widget that owns this term_area
     FVTerm* pre_proc_instance;
     FPreprocessingHandler pre_proc;
     line_changes* changes;
     char_data* text;     // Text data for the output
     bool input_cursor_visible;
     bool has_changes;
     bool visible;
   } term_area;

   enum covered_state
   {
     non_covered,
     half_covered,
     fully_covered
   };

   // Constructor
   explicit FVTerm (FVTerm* = 0);

   // Destructor
  ~FVTerm();

   // Overloaded operators
   template<class type> FVTerm& operator << (const type&);

   // Accessors
   virtual const char* getClassName() const;
   static short        getTermForegroundColor();
   static short        getTermBackgroundColor();
   term_area*          getVWin() const;
   FPoint              getPrintCursor();

   // Mutators
   static void         setTermXY (register int, register int);
   static bool         hideCursor (bool);
   static bool         hideCursor();
   static bool         showCursor();
   void                setPrintCursor (const FPoint&);
   void                setPrintCursor (register int, register int);
   void                setColor (short, short);
   static void         setNormal();

   static bool         setBold (register bool);
   static bool         setBold();
   static bool         unsetBold();

   static bool         setDim (register bool);
   static bool         setDim();
   static bool         unsetDim();

   static bool         setItalic (register bool);
   static bool         setItalic();
   static bool         unsetItalic();

   static bool         setUnderline (register bool);
   static bool         setUnderline();
   static bool         unsetUnderline();

   static bool         setBlink (register bool);
   static bool         setBlink();
   static bool         unsetBlink();

   static bool         setReverse (register bool);
   static bool         setReverse();
   static bool         unsetReverse();

   static bool         setStandout (register bool);
   static bool         setStandout();
   static bool         unsetStandout();

   static bool         setInvisible (register bool);
   static bool         setInvisible();
   static bool         unsetInvisible();

   static bool         setProtected (register bool);
   static bool         setProtected();
   static bool         unsetProtected();

   static bool         setCrossedOut (register bool);
   static bool         setCrossedOut();
   static bool         unsetCrossedOut();

   static bool         setDoubleUnderline (register bool);
   static bool         setDoubleUnderline();
   static bool         unsetDoubleUnderline();

   static bool         setAltCharset (register bool);
   static bool         setAltCharset();
   static bool         unsetAltCharset();

   static bool         setPCcharset (register bool);
   static bool         setPCcharset();
   static bool         unsetPCcharset();

   static bool         setTransparent (register bool);
   static bool         setTransparent();
   static bool         unsetTransparent();

   static bool         setTransShadow (register bool);
   static bool         setTransShadow();
   static bool         unsetTransShadow();

   static bool         setInheritBackground (register bool);
   static bool         setInheritBackground();
   static bool         unsetInheritBackground();

   void                setPreprocessingHandler ( FVTerm*
                                               , FPreprocessingHandler );

   // Inquiries
   static bool         isCursorHidden();
   static bool         isBold();
   static bool         isDim();
   static bool         isItalic();
   static bool         isUnderline();
   static bool         isBlink();
   static bool         isReverse();
   static bool         isStandout();
   static bool         isInvisible();
   static bool         isProtected();
   static bool         isCrossedOut();
   static bool         isDoubleUnderline();
   static bool         isAltCharset();
   static bool         isPCcharset();
   static bool         isTransparent();
   static bool         isTransShadow();
   static bool         isInheritBackground();

   // Methods
   virtual void        clearArea (int = ' ');
   void                createVTerm (const FRect&);
   void                createVTerm (int, int);
   static void         resizeVTerm (const FRect&);
   static void         resizeVTerm (int, int);
   static void         putVTerm();
   static void         updateTerminal (bool);
   static void         updateTerminal();

   int                 printf (const wchar_t*, ...);
   int                 printf (const char*, ...)
   #if defined(__clang__)
     __attribute__((__format__ (__printf__, 2, 3)))
   #elif defined(__GNUC__)
     __attribute__ ((format (printf, 2, 3)))
   #endif
                       ;
   int                 print (const std::wstring&);
   int                 print (term_area*, const std::wstring&);
   int                 print (const wchar_t*);
   int                 print (term_area*, const wchar_t*);
   int                 print (const char*);
   int                 print (term_area*, const char*);
   int                 print (const std::string&);
   int                 print (term_area*, const std::string&);
   int                 print (FString&);
   int                 print (term_area*, FString&);
   int                 print (int);
   int                 print (term_area*, int);
   FVTerm&             print();

 protected:
   // Enumeration
   enum character_type
   {
     overlapped_character,
     covered_character
   };

   // Accessor
   virtual term_area*   getPrintArea();

   // Inquiries
   bool                 hasPrintArea() const;
   bool                 hasChildPrintArea() const;
   bool                 isVirtualWindow() const;

   // Methods
   void                 createArea ( const FRect&
                                   , const FPoint&
                                   , term_area*& );

   void                 createArea ( int, int, int, int
                                   , int, int
                                   , term_area*& );

   static void          resizeArea ( const FRect&
                                   , const FPoint&
                                   , term_area* );

   static void          resizeArea ( int, int, int, int
                                   , int, int
                                   , term_area* );

   static void          removeArea (term_area*&);
   static void          restoreVTerm (const FRect&);
   static void          restoreVTerm (int, int, int, int);

   static covered_state isCovered ( const FPoint&
                                  , term_area* );

   static covered_state isCovered ( int, int
                                  , term_area* );

   static void          updateVTerm();
   static void          updateVTerm (term_area*);
   static bool          updateVTermCursor (term_area*);
   static bool          isInsideArea (int, int, term_area*);

   static void          setAreaCursor ( const FPoint&
                                      , bool, term_area* );

   static void          setAreaCursor ( int, int
                                      , bool, term_area*);

   static void          getArea (const FPoint&, term_area*);
   static void          getArea (int, int, term_area*);
   static void          getArea (const FRect&, term_area*);
   static void          getArea (int, int, int, int, term_area*);
   static void          putArea (const FPoint&, term_area*);
   static void          putArea (int, int, term_area*);
   static void          scrollAreaForward (term_area*);
   static void          scrollAreaReverse (term_area*);
   static void          clearArea (term_area*, int = ' ');

   static char_data     getCharacter ( character_type
                                     , const FPoint&
                                     , FVTerm* );

   static char_data     getCharacter ( character_type
                                     , int
                                     , int, FVTerm* );

   static char_data     getCoveredCharacter (const FPoint&, FVTerm*);
   static char_data     getCoveredCharacter (int, int, FVTerm*);
   static char_data     getOverlappedCharacter (const FPoint&, FVTerm*);
   static char_data     getOverlappedCharacter (int, int, FVTerm*);
   static void          processTerminalUpdate();
   static void          startTerminalUpdate();
   static void          finishTerminalUpdate();
   static void          flush_out();

   // Data Members
   static        term_area* vterm;        // virtual terminal
   static        term_area* vdesktop;     // virtual desktop
   static        term_area* active_area;  // active area
   term_area*    print_area;              // print area for this object
   term_area*    child_print_area;        // print area for children
   term_area*    vwin;                    // virtual window

 private:
   // Typedef
   typedef FTermcap::tcap_map termcap_map;

   // Constants
   static const uInt TERMINAL_OUTPUT_BUFFER_SIZE = 32768;
   // Buffer size for character output on the terminal

   // Disable copy constructor
   FVTerm (const FVTerm&);

   // Disable assignment operator (=)
   FVTerm& operator = (const FVTerm&);

   // Mutators
   void                    setPrintArea (term_area*);

   // Methods
   void                    init();
   void                    finish();
   static bool             clearTerm (int = ' ');
   static void             updateTerminalLine (uInt);
   static bool             updateTerminalCursor();
   static bool             isInsideTerminal (int, int);
   static void             markAsPrinted (uInt, uInt);
   static void             markAsPrinted (uInt, uInt, uInt);
   static void             newFontChanges (char_data*&);
   static void             charsetChanges (char_data*&);
   static void             appendCharacter (char_data*&);
   static void             appendChar (char_data*&);
   static void             appendAttributes (char_data*&);
   static int              appendLowerRight (char_data*&);
   static void             appendOutputBuffer (std::string&);
   static void             appendOutputBuffer (const char*);
   static int              appendOutputBuffer (int);

   // Data Members
   static std::queue<int>* output_buffer;
   static char_data        term_attribute;
   static char_data        next_attribute;
   static FPoint*          term_pos;  // terminal cursor position
   static termcap_map*     tcap;
   static bool             hidden_cursor;
   static bool             terminal_update_complete;
   static bool             terminal_update_pending;
   static bool             force_terminal_update;
   static bool             stop_terminal_updates;
   static int              skipped_terminal_update;
};

#pragma pack(pop)

// FVTerm inline functions
//----------------------------------------------------------------------
template<class type>
inline FVTerm& FVTerm::operator << (const type& s)
{
  std::ostringstream outstream;
  outstream << s;
  print (outstream.str());
  return *this;
}

//----------------------------------------------------------------------
inline const char* FVTerm::getClassName() const
{ return "FVTerm"; }

//----------------------------------------------------------------------
inline short FVTerm::getTermForegroundColor()
{ return next_attribute.fg_color; }

//----------------------------------------------------------------------
inline short FVTerm::getTermBackgroundColor()
{ return next_attribute.bg_color; }

//----------------------------------------------------------------------
inline FVTerm::term_area* FVTerm::getVWin() const
{ return vwin; }

//----------------------------------------------------------------------
inline bool FVTerm::hideCursor()
{ return hideCursor(true); }

//----------------------------------------------------------------------
inline bool FVTerm::showCursor()
{ return hideCursor(false); }

//----------------------------------------------------------------------
inline void FVTerm::setPrintCursor (const FPoint& pos)
{ setPrintCursor (pos.getX(), pos.getY()); }

//----------------------------------------------------------------------
inline void FVTerm::setColor (register short fg, register short bg)
{
  // Changes colors
  next_attribute.fg_color = fg;
  next_attribute.bg_color = bg;
}

//----------------------------------------------------------------------
inline void FVTerm::setNormal()
{
  // reset all character attributes
  next_attribute.bold          = \
  next_attribute.dim           = \
  next_attribute.italic        = \
  next_attribute.underline     = \
  next_attribute.blink         = \
  next_attribute.reverse       = \
  next_attribute.standout      = \
  next_attribute.invisible     = \
  next_attribute.protect       = \
  next_attribute.crossed_out   = \
  next_attribute.dbl_underline = \
  next_attribute.alt_charset   = \
  next_attribute.pc_charset    = \
  next_attribute.transparent   = \
  next_attribute.trans_shadow  = \
  next_attribute.inherit_bg    = \
  next_attribute.no_changes    = false;

  next_attribute.fg_color      = fc::Default;
  next_attribute.bg_color      = fc::Default;
}

//----------------------------------------------------------------------
inline bool FVTerm::setBold (register bool on)
{ return (next_attribute.bold = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setBold()
{ return setBold(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetBold()
{ return setBold(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setDim (register bool on)
{ return (next_attribute.dim = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setDim()
{ return setDim(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetDim()
{ return setDim(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setItalic (register bool on)
{ return (next_attribute.italic = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setItalic()
{ return setItalic(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetItalic()
{ return setItalic(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setUnderline (register bool on)
{ return (next_attribute.underline = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setUnderline()
{ return setUnderline(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetUnderline()
{ return setUnderline(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setBlink (register bool on)
{ return (next_attribute.blink = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setBlink()
{ return setBlink(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetBlink()
{ return setBlink(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setReverse (register bool on)
{ return (next_attribute.reverse = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setReverse()
{ return setReverse(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetReverse()
{ return setReverse(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setStandout (register bool on)
{ return (next_attribute.standout = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setStandout()
{ return setStandout(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetStandout()
{ return setStandout(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setInvisible (register bool on)
{ return (next_attribute.invisible = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setInvisible()
{ return setInvisible(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetInvisible()
{ return setInvisible(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setProtected (register bool on)
{ return (next_attribute.protect = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setProtected()
{ return setProtected(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetProtected()
{ return setProtected(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setCrossedOut (register bool on)
{ return (next_attribute.crossed_out = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setCrossedOut()
{ return setCrossedOut(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetCrossedOut()
{ return setCrossedOut(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setDoubleUnderline (register bool on)
{ return (next_attribute.dbl_underline = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setDoubleUnderline()
{ return setDoubleUnderline(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetDoubleUnderline()
{ return setDoubleUnderline(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setAltCharset (register bool on)
{ return (next_attribute.alt_charset = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setAltCharset()
{ return setAltCharset(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetAltCharset()
{ return setAltCharset(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setPCcharset (register bool on)
{ return (next_attribute.pc_charset = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setPCcharset()
{ return setPCcharset(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetPCcharset()
{ return setPCcharset(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setTransparent (register bool on)
{ return (next_attribute.transparent = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setTransparent()
{ return setTransparent(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetTransparent()
{ return setTransparent(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setTransShadow (register bool on)
{ return (next_attribute.trans_shadow = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setTransShadow()
{ return setTransShadow(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetTransShadow()
{ return setTransShadow(false); }

//----------------------------------------------------------------------
inline bool FVTerm::setInheritBackground (register bool on)
{ return (next_attribute.inherit_bg = on); }

//----------------------------------------------------------------------
inline bool FVTerm::setInheritBackground()
{ return setInheritBackground(true); }

//----------------------------------------------------------------------
inline bool FVTerm::unsetInheritBackground()
{ return setInheritBackground(false); }

//----------------------------------------------------------------------
inline bool FVTerm::isCursorHidden()
{ return hidden_cursor; }

//----------------------------------------------------------------------
inline bool FVTerm::isBold()
{ return next_attribute.bold; }

//----------------------------------------------------------------------
inline bool FVTerm::isDim()
{ return next_attribute.dim; }

//----------------------------------------------------------------------
inline bool FVTerm::isItalic()
{ return next_attribute.italic; }

//----------------------------------------------------------------------
inline bool FVTerm::isUnderline()
{ return next_attribute.underline; }

//----------------------------------------------------------------------
inline bool FVTerm::isBlink()
{ return next_attribute.blink; }

//----------------------------------------------------------------------
inline bool FVTerm::isReverse()
{ return next_attribute.reverse; }

//----------------------------------------------------------------------
inline bool FVTerm::isStandout()
{ return next_attribute.standout; }

//----------------------------------------------------------------------
inline bool FVTerm::isInvisible()
{ return next_attribute.invisible; }

//----------------------------------------------------------------------
inline bool FVTerm::isProtected()
{ return next_attribute.protect; }

//----------------------------------------------------------------------
inline bool FVTerm::isCrossedOut()
{ return next_attribute.crossed_out; }

//----------------------------------------------------------------------
inline bool FVTerm::isDoubleUnderline()
{ return next_attribute.dbl_underline; }

//----------------------------------------------------------------------
inline bool FVTerm::isAltCharset()
{ return next_attribute.alt_charset; }

//----------------------------------------------------------------------
inline bool FVTerm::isPCcharset()
{ return next_attribute.pc_charset; }

//----------------------------------------------------------------------
inline bool FVTerm::isTransparent()
{ return next_attribute.transparent; }

//----------------------------------------------------------------------
inline bool FVTerm::isTransShadow()
{ return next_attribute.trans_shadow; }

//----------------------------------------------------------------------
inline bool FVTerm::isInheritBackground()
{ return next_attribute.inherit_bg; }

//----------------------------------------------------------------------
inline FVTerm& FVTerm::print()
{ return *this; }

//----------------------------------------------------------------------
inline bool FVTerm::hasPrintArea() const
{ return print_area; }

//----------------------------------------------------------------------
inline bool FVTerm::hasChildPrintArea() const
{ return child_print_area; }

//----------------------------------------------------------------------
inline bool FVTerm::isVirtualWindow() const
{ return vwin; }

//----------------------------------------------------------------------
inline void FVTerm::setPrintArea (term_area* area)
{ print_area = area; }

#endif  // _FVTERM_H
