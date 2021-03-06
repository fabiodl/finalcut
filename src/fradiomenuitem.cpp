// File: fradiomenuitem.cpp
// Provides: class FRadioMenuItem

#include "fradiomenuitem.h"
#include "fmenu.h"

//----------------------------------------------------------------------
// class FRadioMenuItem
//----------------------------------------------------------------------

// constructor and destructor
//----------------------------------------------------------------------
FRadioMenuItem::FRadioMenuItem (FWidget* parent)
  : FMenuItem(parent)
{
  init (parent);
}

//----------------------------------------------------------------------
FRadioMenuItem::FRadioMenuItem (FString& txt, FWidget* parent)
  : FMenuItem(txt, parent)
{
  init (parent);
}

//----------------------------------------------------------------------
FRadioMenuItem::FRadioMenuItem (const std::string& txt, FWidget* parent)
  : FMenuItem(txt, parent)
{
  init (parent);
}

//----------------------------------------------------------------------
FRadioMenuItem::FRadioMenuItem (const char* txt, FWidget* parent)
  : FMenuItem(txt, parent)
{
  init (parent);
}

//----------------------------------------------------------------------
FRadioMenuItem::~FRadioMenuItem()  // destructor
{ }


// private methods of FRadioMenuItem
//----------------------------------------------------------------------
void FRadioMenuItem::init (FWidget* parent)
{
  checkable = true;
  radio_button = true;

  if ( ! parent )
    return;

  if ( isMenu(parent) ) // Parent is menu
  {
    FMenu* menu_ptr = dynamic_cast<FMenu*>(parent);

    if ( menu_ptr )
      menu_ptr->has_checkable_items = true;

    this->addCallback
    (
      "toggled",
      _METHOD_CALLBACK (parent, &FMenu::cb_menuitem_toggled)
    );
  }
}

//----------------------------------------------------------------------
void FRadioMenuItem::processToggle()
{
  emitCallback("toggled");
}

//----------------------------------------------------------------------
void FRadioMenuItem::processClicked()
{
  if ( ! checked )
  {
    checked = true;
    processToggle();
  }

  emitCallback("clicked");
}
