// File: flabel.h
// Provides: class FLabel
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
//       ▕▔▔▔▔▔▔▔▔▏
//       ▕ FVTerm ▏
//       ▕▁▁▁▁▁▁▁▁▏
//            ▲
//            │
//       ▕▔▔▔▔▔▔▔▔▔▏
//       ▕ FWidget ▏
//       ▕▁▁▁▁▁▁▁▁▁▏
//            ▲
//            │
//       ▕▔▔▔▔▔▔▔▔▏
//       ▕ FLabel ▏
//       ▕▁▁▁▁▁▁▁▁▏

#ifndef _FLABEL_H
#define _FLABEL_H

#include "fwidget.h"


//----------------------------------------------------------------------
// class FLabel
//----------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

class FLabel : public FWidget
{
 public:
   // Using-declaration
   using FWidget::setEnable;

   // Constructor
   explicit FLabel (FWidget* = 0);
   FLabel (const FString&, FWidget* = 0);

   // Destructor
   virtual ~FLabel();

   // Accessors
   const char* getClassName() const;
   FTerm*      getAccelWidget();
   uInt        getAlignment();
   FString&    getText();

   // Mutators
   void        setAccelWidget (FWidget* = 0);
   void        setAlignment(uInt);
   bool        setEmphasis(bool);
   bool        setEmphasis();
   bool        unsetEmphasis();
   bool        setReverseMode(bool);
   bool        setReverseMode();
   bool        unsetReverseMode();
   bool        setEnable (bool);
   void        setNumber(long);
   void        setText (const FString&);

   // Inquiries
   bool        hasEmphasis();
   bool        hasReverseMode();

   // Methods
   void        hide();

   // Event handlers
   void        onMouseDown (FMouseEvent*);
   void        onAccel (FAccelEvent*);

   // Callback method
   void        cb_accel_widget_destroyed (FWidget*, void*);

 private:
   // Typedef
   typedef std::vector<FString> multiLineText;

   // Disable copy constructor
   FLabel (const FLabel&);

   // Disable assignment operator (=)
   FLabel& operator = (const FLabel&);

   // Methods
   void          init();
   uChar         getHotkey();
   int           getHotkeyPos (wchar_t*&, wchar_t*&, uInt);
   void          setHotkeyAccelerator();
   int           getXOffset (int);
   void          printLine (wchar_t*&, uInt, int, int = 0);
   void          draw();

   // Data Members
   multiLineText multiline_text;
   bool          multiline;
   FString       text;
   uInt          alignment;
   short         emphasis_color;
   short         ellipsis_color;
   bool          emphasis;
   bool          reverse_mode;
   FWidget*      accel_widget;
};
#pragma pack(pop)


// FLabel inline functions
//----------------------------------------------------------------------
inline const char* FLabel::getClassName() const
{ return "FLabel"; }

//----------------------------------------------------------------------
inline FTerm* FLabel::getAccelWidget ()
{ return accel_widget; }

//----------------------------------------------------------------------
inline uInt FLabel::getAlignment()
{ return alignment; }

//----------------------------------------------------------------------
inline FString& FLabel::getText()
{ return text; }

//----------------------------------------------------------------------
inline bool FLabel::setEmphasis()
{ return setEmphasis(true); }

//----------------------------------------------------------------------
inline bool FLabel::unsetEmphasis()
{ return setEmphasis(false); }

//----------------------------------------------------------------------
inline bool FLabel::setReverseMode()
{ return setReverseMode(true); }

//----------------------------------------------------------------------
inline bool FLabel::unsetReverseMode()
{ return setReverseMode(false); }

//----------------------------------------------------------------------
inline bool FLabel::hasEmphasis()
{ return emphasis; }

//----------------------------------------------------------------------
inline bool FLabel::hasReverseMode()
{ return reverse_mode; }

#endif  // _FLABEL_H
