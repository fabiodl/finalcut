// File: fbuttongroup.cpp
// Provides: class FButtonGroup

#include "fapp.h"
#include "fbuttongroup.h"
#include "fstatusbar.h"
#include "ftogglebutton.h"


//----------------------------------------------------------------------
// class FButtonGroup
//----------------------------------------------------------------------

// constructor and destructor
//----------------------------------------------------------------------
FButtonGroup::FButtonGroup(FWidget* parent)
  : FWidget(parent)
  , text()
  , border(true)
  , buttonlist()
{
  init();
}

//----------------------------------------------------------------------
FButtonGroup::FButtonGroup (const FString& txt, FWidget* parent)
  : FWidget(parent)
  , text(txt)
  , border(true)
  , buttonlist()
{
  init();
  setText(txt);
}

//----------------------------------------------------------------------
FButtonGroup::~FButtonGroup()  // destructor
{
  FObjectList::iterator iter;

  if ( buttonlist.empty() )
    return;

  iter = buttonlist.begin();

  while ( iter != buttonlist.end() )
  {
    FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);
    toggle_button->setGroup(0);
    iter = buttonlist.erase(iter);
  }
}


// public methods of FButtonGroup
//----------------------------------------------------------------------
FToggleButton* FButtonGroup::getFirstButton()
{
  FWidget* widget = FWidget::getFirstFocusableWidget(buttonlist);
  FToggleButton* toggle_button = static_cast<FToggleButton*>(widget);
  return toggle_button;
}

//----------------------------------------------------------------------
FToggleButton* FButtonGroup::getLastButton()
{
  FWidget* widget = FWidget::getLastFocusableWidget(buttonlist);
  FToggleButton* toggle_button = static_cast<FToggleButton*>(widget);
  return toggle_button;
}

//----------------------------------------------------------------------
bool FButtonGroup::setEnable (bool on)
{
  FWidget::setEnable(on);

  if ( on )
  {
    flags |= fc::active;
    setHotkeyAccelerator();
  }
  else
  {
    flags &= ~fc::active;
    delAccelerator();
  }

  return on;
}

//----------------------------------------------------------------------
bool FButtonGroup::setBorder(bool on)
{
  if ( on )
    border = true;
  else
    border = false;

  return on;
}

//----------------------------------------------------------------------
void FButtonGroup::setText (const FString& txt)
{
  text = txt;

  if ( isEnabled() )
  {
    delAccelerator();
    setHotkeyAccelerator();
  }
}

//----------------------------------------------------------------------
bool FButtonGroup::hasFocusedButton()
{
  if ( buttonlist.empty() )
    return false;

  FObjectList::const_iterator iter, end;
  iter = buttonlist.begin();
  end = buttonlist.end();

  while ( iter != end )
  {
    FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);

    if ( toggle_button->hasFocus() )
      return true;

    ++iter;
  }

  return false;
}

//----------------------------------------------------------------------
bool FButtonGroup::hasCheckedButton()
{
  if ( buttonlist.empty() )
    return false;

  FObjectList::const_iterator iter, end;
  iter = buttonlist.begin();
  end = buttonlist.end();

  while ( iter != end )
  {
    FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);

    if ( toggle_button->isChecked() )
      return true;

    ++iter;
  }

  return false;
}

//----------------------------------------------------------------------
void FButtonGroup::hide()
{
  int size;
  short fg, bg;
  char* blank;
  FWidget::hide();
  FWidget* parent_widget = getParentWidget();

  if ( ! buttonlist.empty() )
  {
    FObjectList::const_iterator iter, end;
    iter = buttonlist.begin();
    end = buttonlist.end();

    while ( iter != end )
    {
      FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);
      toggle_button->hide();
      ++iter;
    }
  }

  if ( parent_widget )
  {
    fg = parent_widget->getForegroundColor();
    bg = parent_widget->getBackgroundColor();
  }
  else
  {
    fg = wc.dialog_fg;
    bg = wc.dialog_bg;
  }

  setColor (fg, bg);
  size = getWidth();

  if ( size < 0 )
    return;

  blank = new char[size+1];
  std::memset(blank, ' ', uLong(size));
  blank[size] = '\0';

  for (int y=0; y < getHeight(); y++)
  {
    setPrintPos (1, 1+y);
    print (blank);
  }

  delete[] blank;
}

//----------------------------------------------------------------------
void FButtonGroup::insert (FToggleButton* button)
{
  if ( ! button )
    return;

  if ( button->getGroup() )
    button->getGroup()->remove(button);

  // setChecked the first FRadioButton
  if ( buttonlist.size() == 1 )
  {
    FToggleButton* first_button = static_cast<FToggleButton*>(*buttonlist.begin());

    if ( isRadioButton(first_button) )
      first_button->setChecked();
  }

  button->setGroup(this);
  buttonlist.push_back(button);

  button->addCallback
  (
    "toggled",
    _METHOD_CALLBACK (this, &FButtonGroup::cb_buttonToggled)
  );
}

//----------------------------------------------------------------------
void FButtonGroup::remove (FToggleButton* button)
{
  FObjectList::iterator iter;

  if ( ! button || buttonlist.empty() )
    return;

  iter = buttonlist.begin();

  while ( iter != buttonlist.end() )
  {
    FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);

    if ( toggle_button == button )
    {
      iter = buttonlist.erase(iter);
      button->setGroup(0);
      button->delCallback(this);
      break;
    }
    else
      ++iter;
  }
}

//----------------------------------------------------------------------
void FButtonGroup::onMouseDown (FMouseEvent* ev)
{
  if ( ev->getButton() != fc::LeftButton )
    return;

  directFocus();
}

//----------------------------------------------------------------------
void FButtonGroup::onAccel (FAccelEvent*)
{
  directFocus();
}

//----------------------------------------------------------------------
void FButtonGroup::onFocusIn (FFocusEvent* in_ev)
{
  if ( hasCheckedButton() && ! buttonlist.empty() )
  {
    FObjectList::const_iterator iter, end;
    iter = buttonlist.begin();
    end = buttonlist.end();

    while ( iter != end )
    {
      FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);

      if ( toggle_button->isChecked() )
      {
        if ( isRadioButton(toggle_button) )
        {
          in_ev->ignore();
          FWidget* prev_element = getFocusWidget();
          toggle_button->setFocus();

          if ( prev_element )
            prev_element->redraw();

          toggle_button->redraw();
        }

        break;
      }

      ++iter;
    }
  }

  if ( in_ev->isAccepted() )
  {
    in_ev->ignore();
    FWidget* prev_element = getFocusWidget();

    if ( in_ev->getFocusType() == fc::FocusNextWidget )
      focusFirstChild();
    else if ( in_ev->getFocusType() == fc::FocusPreviousWidget )
      focusLastChild();

    if ( prev_element )
      prev_element->redraw();

    if ( getFocusWidget() )
      getFocusWidget()->redraw();
  }

  if ( getStatusBar() )
  {
    getStatusBar()->drawMessage();
    updateTerminal();
    flush_out();
  }
}

//----------------------------------------------------------------------
void FButtonGroup::cb_buttonToggled (FWidget* widget, void*)
{
  FToggleButton* button = static_cast<FToggleButton*>(widget);
  FObjectList::const_iterator iter, end;

  if ( ! button->isChecked() )
    return;

  if ( buttonlist.empty() )
    return;

  iter = buttonlist.begin();
  end = buttonlist.end();

  while ( iter != end )
  {
    FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);

    if ( toggle_button != button
        && toggle_button->isChecked()
        && isRadioButton(toggle_button) )
    {
      toggle_button->unsetChecked();

      if ( toggle_button->isVisible() && toggle_button->isShown() )
        toggle_button->redraw();
    }

    ++iter;
  }
}


// protected methods of FButtonGroup
//----------------------------------------------------------------------
uChar FButtonGroup::getHotkey()
{
  uInt length;

  if ( text.isEmpty() )
    return 0;

  length = text.getLength();

  for (uInt i=0; i < length; i++)
  {
    try
    {
      if ( (i+1 < length) && (text[i] == '&') )
        return uChar(text[++i]);
    }
    catch (const std::out_of_range&)
    {
      return 0;;
    }
  }
  return 0;
}

//----------------------------------------------------------------------
void FButtonGroup::setHotkeyAccelerator()
{
  int hotkey = getHotkey();

  if ( hotkey )
  {
    if ( std::isalpha(hotkey) || std::isdigit(hotkey) )
    {
      addAccelerator (std::tolower(hotkey));
      addAccelerator (std::toupper(hotkey));
      // Meta + hotkey
      addAccelerator (fc::Fmkey_meta + std::tolower(hotkey));
    }
    else
      addAccelerator (getHotkey());
  }
  else
    delAccelerator();
}

//----------------------------------------------------------------------
void FButtonGroup::draw()
{
  if ( isMonochron() )
    setReverse(true);

  setColor();

  if ( border )
    drawBorder();

  drawLabel();

  if ( isMonochron() )
    setReverse(false);
}

//----------------------------------------------------------------------
void FButtonGroup::drawLabel()
{
  wchar_t* LabelText;
  register wchar_t* src;
  register wchar_t* dest;
  FString txt;
  uInt length;
  int hotkeypos;
  bool isActive, isNoUnderline;

  if ( text.isNull() || text.isEmpty() )
    return;

  txt = " " + text + " ";
  length = txt.getLength();
  hotkeypos = -1;
  LabelText = new wchar_t[length+1]();
  src = const_cast<wchar_t*>(txt.wc_str());
  dest = const_cast<wchar_t*>(LabelText);

  isActive = ((flags & fc::active) != 0);
  isNoUnderline = ((flags & fc::no_underline) != 0);

  // find hotkey position in string
  // + generate a new string without the '&'-sign
  for (uInt i=0; i < length; i++)
  {
    if ( (i < length) && (txt[i] == '&') && (hotkeypos == -1) )
    {
      hotkeypos = int(i);
      i++;
      src++;
    }
    *dest++ = *src++;
  }

  if ( hotkeypos != -1 )
    length--;

  if ( border )
    setPrintPos (2, 1);
  else
    setPrintPos (0, 1);

  if ( isEnabled() )
    setColor(wc.label_emphasis_fg, wc.label_bg);
  else
    setColor(wc.label_inactive_fg, wc.label_inactive_bg);

  for (int z=0; z < int(length); z++)
  {
    if ( (z == hotkeypos) && isActive )
    {
      setColor (wc.label_hotkey_fg, wc.label_hotkey_bg);

      if ( ! isNoUnderline )
        setUnderline();

      print ( LabelText[z] );

      if ( ! isNoUnderline )
        unsetUnderline();

      setColor (wc.label_emphasis_fg, wc.label_bg);
    }
    else
      print ( LabelText[z] );
  }

  delete[] LabelText;
}


// private methods of FButtonGroup
//----------------------------------------------------------------------
bool FButtonGroup::isRadioButton(FToggleButton* button) const
{
  if ( ! button )
    return false;

  return bool ( std::strcmp ( button->getClassName()
                            , const_cast<char*>("FRadioButton") ) == 0 );
}

//----------------------------------------------------------------------
void FButtonGroup::init()
{
  setTopPadding(1);
  setLeftPadding(1);
  setBottomPadding(1);
  setRightPadding(1);

  if ( isEnabled() )
    flags |= fc::active;

  setForegroundColor (wc.label_fg);
  setBackgroundColor (wc.label_bg);
  buttonlist.clear();  // no buttons yet
}

//----------------------------------------------------------------------
void FButtonGroup::directFocus()
{
  if ( ! hasFocusedButton() )
  {
    bool found_checked = false;

    if ( hasCheckedButton() && ! buttonlist.empty() )
    {
      FObjectList::const_iterator iter, end;
      iter = buttonlist.begin();
      end = buttonlist.end();

      while ( iter != end )
      {
        FToggleButton* toggle_button = static_cast<FToggleButton*>(*iter);

        if ( toggle_button->isChecked() )
        {
          if ( isRadioButton(toggle_button) )
          {
            found_checked = true;
            FWidget* focused_widget = getFocusWidget();
            FFocusEvent out (fc::FocusOut_Event);
            FApplication::queueEvent(focused_widget, &out);
            toggle_button->setFocus();

            if ( focused_widget )
              focused_widget->redraw();

            focused_widget = getFocusWidget();

            if ( focused_widget )
              focused_widget->redraw();
          }

          break;
        }

        ++iter;
      }
    }

    if ( ! found_checked )
    {
      FWidget* focused_widget = getFocusWidget();
      FFocusEvent out (fc::FocusOut_Event);
      FApplication::queueEvent(focused_widget, &out);
      focusFirstChild();

      if ( focused_widget )
        focused_widget->redraw();

      focused_widget = getFocusWidget();

      if ( focused_widget )
        focused_widget->redraw();
    }
  }

  if ( getStatusBar() )
  {
    getStatusBar()->drawMessage();
    updateTerminal();
    flush_out();
  }
}
