// File: fradiobutton.cpp
// Provides: class FRadioButton

#include "fapp.h"
#include "fradiobutton.h"


//----------------------------------------------------------------------
// class FRadioButton
//----------------------------------------------------------------------

// constructor and destructor
//----------------------------------------------------------------------
FRadioButton::FRadioButton(FWidget* parent)
  : FToggleButton(parent)
{
  init();
}

//----------------------------------------------------------------------
FRadioButton::FRadioButton ( const FString& txt, FWidget* parent )
  : FToggleButton(txt, parent)
{
  init();
}

//----------------------------------------------------------------------
FRadioButton::~FRadioButton()  // destructor
{ }


// private methods of FRadioButton
//----------------------------------------------------------------------
void FRadioButton::init()
{
  label_offset_pos = 4;
  button_width = 4;
  setVisibleCursor();
}

//----------------------------------------------------------------------
void FRadioButton::draw()
{
  drawRadioButton();
  drawLabel();
  FToggleButton::draw();
}

//----------------------------------------------------------------------
void FRadioButton::drawRadioButton()
{
  if ( ! isVisible() )
    return;

  setPrintPos (1,1);
  setColor();

  if ( isMonochron() )
  {
    if ( hasFocus() )
      setReverse(false);
    else
      setReverse(true);
  }

  if ( checked )
  {
    if ( isNewFont() )
      print (CHECKED_RADIO_BUTTON);
    else
    {
      print ('(');
      if ( isCygwinTerminal() )
        print (0x04);
      else
        print (fc::Bullet);  // Bullet ●
      print (')');
    }
  }
  else
  {
    if ( isNewFont() )
      print (RADIO_BUTTON);
    else
    {
      print ('(');
      print (' ');
      print (')');
    }
  }

  if ( isMonochron() )
    setReverse(false);
}
