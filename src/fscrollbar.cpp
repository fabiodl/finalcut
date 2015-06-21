// fscrollbar.cpp
// class FScrollbar

#include "fscrollbar.h"

//----------------------------------------------------------------------
// class FScrollbar
//----------------------------------------------------------------------

// constructors and destructor
//----------------------------------------------------------------------
FScrollbar::FScrollbar(FWidget* parent) : FWidget(parent)
{
  this->init();
}

//----------------------------------------------------------------------
FScrollbar::FScrollbar(int o, FWidget* parent) : FWidget(parent)
{
  this->init();
  setOrientation (o);
}

//----------------------------------------------------------------------
FScrollbar::~FScrollbar()
{
  delAllTimer();
}

// private methods of FScrollbar
//----------------------------------------------------------------------
void FScrollbar::init()
{
  SliderPos = 0;
  SliderClickPos = -1;
  currentSliderPos = -1;
  min = val = 0;
  max = 99;
  steps = 1;
  pageSize = 0;
  bar_orientation = fc::vertical;
  width = 1;
  length = height = 20;
  BarLength = length - 2;
  SliderLength = BarLength = 20-2;
  setGeometry(1, 1, 1, 20);
  scrollType = FScrollbar::noScroll;
  thresholdTime = 500;
  repeatTime = 10;
  max_color = getMaxColor();
  ignore_padding = true;
  unsetFocusable();
}

//----------------------------------------------------------------------
void FScrollbar::draw()
{
  //setUpdateVTerm(false);
  drawButtons();
  currentSliderPos = -1;
  drawBar();
  //setUpdateVTerm(true);
}

//----------------------------------------------------------------------
int FScrollbar::getClickedScrollType (int x, int y)
{
  int stype;
  if ( bar_orientation == fc::vertical )
  {
    if ( y == 1 )
    {
      stype = FScrollbar::scrollStepBackward;  // decrement button
    }
    else if ( y >1 && y <= SliderPos+1 )
    {
      stype = FScrollbar::scrollPageBackward;  // before slider
    }
    else if ( y > SliderPos+SliderLength+1 && y < height )
    {
      stype = FScrollbar::scrollPageForward;  // after slider
    }
    else if (  y == height )
    {
      stype = FScrollbar::scrollStepForward; // increment button
    }
    else
      stype = FScrollbar::noScroll;
  }
  else  // horizontal
  {
    if ( isNewFont() )
    {
      if ( x == 1 || x == 2 )
      {
        stype = FScrollbar::scrollStepBackward;  // decrement button
      }
      else if ( x >2 && x <= SliderPos+2 )
      {
        stype = FScrollbar::scrollPageBackward;  // before slider
      }
      else if ( x > SliderPos+SliderLength+2 && x < width-1 )
      {
        stype = FScrollbar::scrollPageForward;  // after slider
      }
      else if ( x == width-1 || x == width )
      {
        stype = FScrollbar::scrollStepForward;  // increment button
      }
      else
        stype = FScrollbar::noScroll;
    }
    else
    {
      if ( x == 1 )
      {
        stype = FScrollbar::scrollStepBackward;  // decrement button
      }
      else if ( x >1 && x <= SliderPos+1 )
      {
        stype = FScrollbar::scrollPageBackward;  // before slider
      }
      else if ( x > SliderPos+SliderLength+1 && x < width )
      {
        stype = FScrollbar::scrollPageForward;  // after slider
      }
      else if ( x == width )
      {
        stype = FScrollbar::scrollStepForward;  // increment button
      }
      else
        stype = FScrollbar::noScroll;
    }
  }
  return stype;
}

//----------------------------------------------------------------------
void FScrollbar::processMiddleButton (int x, int y)
{
    int new_val;
    if ( bar_orientation == fc::vertical )
    {
      if ( y >1 && y < height )
      {
        new_val = int( round ( float(max-min) * (y-2-(SliderLength/2))
                             / float(BarLength-SliderLength) ) );
      }
      else
        return;
    }
    else  // horizontal
    {
      int nf = isNewFont() ? 1 : 0;
      if ( x > 1+nf && x < width-nf )
      {
        new_val = int( round ( float(max-min) * (x-2-nf-(SliderLength/2))
                             / float(BarLength-SliderLength) ) );
      }
      else
        return;
    }
    if ( new_val != val )
    {
      setValue(new_val);
      drawBar();
      updateTerminal();
      scrollType = FScrollbar::scrollJump;
      processScroll();
    }
}


//----------------------------------------------------------------------
void FScrollbar::processScroll()
{
  emitCallback("change-value");
}


// public methods of FScrollbar
//----------------------------------------------------------------------
void FScrollbar::onMouseDown (FMouseEvent* event)
{
  int mouse_x, mouse_y;

  if (  event->getButton() != LeftButton
     && event->getButton() != MiddleButton )
    return;

  if ( min == max )
    return;

  mouse_x = event->getX();
  mouse_y = event->getY();

  if ( event->getButton() == MiddleButton )
  {
    processMiddleButton (mouse_x, mouse_y);
    return;
  }

  // process LeftButton
  scrollType = getClickedScrollType(mouse_x, mouse_y);

  if ( bar_orientation == fc::vertical )
  {
    if ( mouse_y > SliderPos+1 && mouse_y <= SliderPos+SliderLength+1 )
      SliderClickPos = mouse_y;  // on slider
  }
  else  // horizontal
  {
    if ( isNewFont() )
    {
      if ( mouse_x > SliderPos+2 && mouse_x <= SliderPos+SliderLength+2 )
        SliderClickPos = mouse_x;  // on slider
    }
    else
    {
      if ( mouse_x > SliderPos+1 && mouse_x <= SliderPos+SliderLength+1 )
        SliderClickPos = mouse_x;  // on slider
    }
  }

  if ( SliderClickPos > 0 )
    scrollType = FScrollbar::scrollJump;

  if (  scrollType == FScrollbar::scrollPageBackward
     || scrollType == FScrollbar::scrollPageForward )
  {
    if ( bar_orientation == fc::vertical )
      SliderClickStopPos = mouse_y - 2;
    else
      if ( isNewFont() )
        SliderClickStopPos = mouse_x - 3;
      else
        SliderClickStopPos = mouse_x - 2;
  }
  else
    SliderClickStopPos = -1;

  if (  scrollType >= FScrollbar::scrollStepBackward
     && scrollType <= FScrollbar::scrollPageForward )
  {
    processScroll();
    thresholdReached = false;
    addTimer(thresholdTime);
  }
}

//----------------------------------------------------------------------
void FScrollbar::onMouseUp (FMouseEvent* event)
{
  if (  event->getButton() != LeftButton
     && event->getButton() != MiddleButton )
    return;

  SliderClickPos = -1;

  if ( scrollType != FScrollbar::noScroll )
  {
    delAllTimer();
    scrollType = FScrollbar::noScroll;
  }
}

//----------------------------------------------------------------------
void FScrollbar::onMouseMove (FMouseEvent* event)
{
  int mouse_x, mouse_y, newScrollType;

  if (  event->getButton() != LeftButton
     && event->getButton() != MiddleButton )
    return;

  mouse_x = event->getX();
  mouse_y = event->getY();

  if ( event->getButton() == MiddleButton )
  {
    processMiddleButton (mouse_x, mouse_y);
    return;
  }

  // process LeftButton
  newScrollType = getClickedScrollType(mouse_x, mouse_y);

  if ( scrollType == FScrollbar::scrollJump )
  {
    int new_val;
    if ( bar_orientation == fc::vertical )
    {
      int dy = mouse_y - SliderClickPos;
      SliderClickPos = mouse_y;

      new_val = int( round ( float(max-min) * (SliderPos + dy)
                           / float(BarLength-SliderLength) ) );
    }
    else  // horizontal
    {
      int dx = mouse_x - SliderClickPos;
      SliderClickPos = mouse_x;

      new_val = int( round ( float(max-min) * (SliderPos + dx)
                           / float(BarLength-SliderLength) ) );
    }
    if ( new_val != val )
    {
      setValue(new_val);
      drawBar();
      updateTerminal();
      processScroll();
    }
  }

  if (  mouse_x < 1 || mouse_x > width
     || mouse_y < 1 || mouse_y > height )
  {
    delAllTimer();
  }
  else if ( scrollType != FScrollbar::scrollJump )
  {
    addTimer(repeatTime);
  }

  if ( scrollType != newScrollType )
  {
    delAllTimer();
  }
}

//----------------------------------------------------------------------
void FScrollbar::onWheel (FWheelEvent* event)
{
  int wheel = event->getWheel();

  if ( scrollType != FScrollbar::noScroll )
  {
    delAllTimer();
    scrollType = FScrollbar::noScroll;
  }

  if ( wheel == WheelUp )
    scrollType = FScrollbar::scrollWheelUp;
  else if ( wheel == WheelDown )
    scrollType = FScrollbar::scrollWheelDown;

  processScroll();
}

//----------------------------------------------------------------------
void FScrollbar::onTimer (FTimerEvent*)
{
  if ( scrollType == FScrollbar::noScroll )
    return;

  if ( ! thresholdReached )
  {
    thresholdReached = true;
    delAllTimer();
    addTimer(repeatTime);
  }

  if (  (  scrollType == scrollPageBackward
        && SliderPos < SliderClickStopPos )
     || (  scrollType == scrollPageForward
        && SliderPos+SliderLength-1 >= SliderClickStopPos ) )
  {
    delAllTimer();
    return;
  }

  processScroll();
}

//----------------------------------------------------------------------
void FScrollbar::resize()
{
  FWidget::resize();
  setOrientation (bar_orientation);
  setValue (val);
  calculateSliderValues();
}

//----------------------------------------------------------------------
void FScrollbar::redraw()
{
  draw();
}

//----------------------------------------------------------------------
void FScrollbar::setMinimum (int minimum)
{
  this->min = minimum;
  calculateSliderValues();
}

//----------------------------------------------------------------------
void FScrollbar::setMaximum (int maximum)
{
  this->max = maximum;
  calculateSliderValues();
}

//----------------------------------------------------------------------
void FScrollbar::setRange(int minimum, int maximum)
{
  this->min = minimum;
  this->max = maximum;
  calculateSliderValues();
}

//----------------------------------------------------------------------
void FScrollbar::setValue (int value)
{
  this->val = value;
  calculateSliderValues();
}

//----------------------------------------------------------------------
void FScrollbar::setSteps (float st)
{
  if ( st <= 0 )
    steps = 1;
  else
    steps = st;
  if ( pageSize == 0 )
    pageSize = int(max/steps);
}

//----------------------------------------------------------------------
void FScrollbar::setPageSize (int document_size, int page_size)
{
  if ( page_size == 0 )
  {
    pageSize = document_size;
    this->steps = 1.0;
  }
  else
  {
    pageSize = page_size;
    this->steps = float(float(document_size) / float(page_size));
  }
}

//----------------------------------------------------------------------
void FScrollbar::calculateSliderValues()
{
  if ( isNewFont() && bar_orientation == fc::horizontal )
    BarLength = length - 4;
  else
    BarLength = length - 2;
  SliderLength = int(float(BarLength / steps));

  if ( SliderLength < 1 )
    SliderLength = 1;
  else if ( SliderLength > BarLength )
    SliderLength = BarLength;

  if ( val == min )
  {
    SliderPos = 0;
    return;
  }
  if ( val == max )
  {
    SliderPos = BarLength - SliderLength;
    return;
  }

  SliderPos = int( round ( float((BarLength-SliderLength) * val)
                         / float(max-min) ) );

  if ( SliderPos < 0 )
    SliderPos = 0;
  else if ( SliderPos > BarLength - SliderLength )
    SliderPos = BarLength - SliderLength;
}

//----------------------------------------------------------------------
void FScrollbar::setOrientation (int o)
{
  int nf = 0;
  length = (height > width) ? height : width;

  if ( o == fc::vertical && bar_orientation == fc::horizontal )
  {
    width = 1;
    height = length;
  }
  else if ( o == fc::horizontal && bar_orientation == fc::vertical )
  {
    height = 1;
    width = length;
    if ( isNewFont() )
      nf = 2;
  }
  SliderLength = BarLength = length-nf-2;
  bar_orientation = o;
}

//----------------------------------------------------------------------
void FScrollbar::setGeometry (int x, int y, int w, int h, bool adjust)
{
  FWidget::setGeometry (x, y, w, h, adjust);

  int nf = 0;
  length = (h > w) ? h : w;

  if ( bar_orientation == fc::vertical )
  {
    width = isNewFont() ? 2 : 1;
    height = length;
  }
  else // horizontal
  {
    height = 1;
    width = length;
    if ( isNewFont() )
      nf = 2;
  }
  SliderLength = BarLength = length-nf-2;
}

//----------------------------------------------------------------------
void FScrollbar::drawBar()
{
  setUpdateVTerm(false);
  if (SliderPos != currentSliderPos)
  {
    int z;
    if ( bar_orientation == fc::vertical )
    {
      setColor (wc.scrollbar_fg, wc.scrollbar_bg);
      for (z=1; z <= SliderPos; z++)
      {
        gotoxy (xpos+xmin-1, ypos+ymin-1+z);
        if ( isNewFont() )
          print (fc::NF_border_line_left); // ⎸
        if ( isMonochron() || max_color < 16 )
          print (fc::MediumShade); // ▒
        else
          print (' ');
      }
      setColor (wc.scrollbar_bg, wc.scrollbar_fg);
      if ( isMonochron() )
        setReverse(true);
      for (z=1; z <= SliderLength; z++)
      {
        gotoxy (xpos+xmin-1, ypos+ymin-1+SliderPos+z);
        if ( isNewFont() )
          print (' ');
        print (' ');
      }
      if ( isMonochron() )
        setReverse(false);
      setColor (wc.scrollbar_fg, wc.scrollbar_bg);
      for (z=SliderPos+SliderLength+1; z <= BarLength; z++)
      {
        gotoxy (xpos+xmin-1, ypos+ymin-1+z);
        if ( isNewFont() )
          print (fc::NF_border_line_left); // ⎸
        if ( isMonochron() || max_color < 16 )
          print (fc::MediumShade);
        else
          print (' ');
      }
    }
    else  // horizontal
    {
      setColor (wc.scrollbar_fg, wc.scrollbar_bg);

      z = 0;
      if ( isNewFont() )
        gotoxy (xpos+xmin+1+z, ypos+ymin-1);
      else
        gotoxy (xpos+xmin+z, ypos+ymin-1);

      for (; z < SliderPos; z++)
      {
        if ( isNewFont() )
          print (fc::NF_border_line_upper); // ¯
        else if ( isMonochron() || max_color < 16 )
          print (fc::MediumShade); // ▒
        else
          print (' ');
      }
      setColor (wc.scrollbar_bg, wc.scrollbar_fg);
      if ( isMonochron() )
        setReverse(true);

      z = 0;
      for (; z < SliderLength; z++)
        print (' ');
      if ( isMonochron() )
        setReverse(false);
      setColor (wc.scrollbar_fg, wc.scrollbar_bg);
      z = SliderPos + SliderLength + 1;
      for (; z <= BarLength; z++)
      {
        if ( isNewFont() )
          print (fc::NF_border_line_upper); // ¯
        else if ( isMonochron() || max_color < 16 )
          print (fc::MediumShade); // ▒
        else
          print (' ');
      }
    }
    currentSliderPos = SliderPos;
  }
  setUpdateVTerm(true);
}

//----------------------------------------------------------------------
void FScrollbar::drawButtons()
{
  setColor (wc.scrollbar_button_fg, wc.scrollbar_button_bg);

  if ( isNewFont() )
  {
    if ( bar_orientation == fc::vertical )
    {
      gotoxy (xpos+xmin-1, ypos+ymin-1);
      print (fc::NF_rev_up_arrow1);
      print (fc::NF_rev_up_arrow2);
      gotoxy (xpos+xmin-1, ypos+ymin+length-2);
      print (fc::NF_rev_down_arrow1);
      print (fc::NF_rev_down_arrow2);
    }
    else // horizontal
    {
      gotoxy (xpos+xmin-1, ypos+ymin-1);
      print (fc::NF_rev_left_arrow1);
      print (fc::NF_rev_left_arrow2);
      gotoxy (xpos+xmin+length-3, ypos+ymin-1);
      print (fc::NF_rev_right_arrow1);
      print (fc::NF_rev_right_arrow2);
    }
  }
  else
  {
    if ( bar_orientation == fc::vertical )
    {
      gotoxy (xpos+xmin-1, ypos+ymin-1);
      if ( isMonochron() )
        setReverse(true);
      if ( isCygwinTerminal() )
        print ('^');
      else
        print (fc::BlackUpPointingTriangle); // ▲
      gotoxy (xpos+xmin-1, ypos+ymin+length-2);
      if ( isCygwinTerminal() )
        print ('v');
      else
        print (fc::BlackDownPointingTriangle); // ▼
      if ( isMonochron() )
        setReverse(false);
    }
    else // horizontal
    {
      gotoxy (xpos+xmin-1, ypos+ymin-1);
      if ( isMonochron() )
        setReverse(true);
      print (fc::BlackLeftPointingPointer); // ◄
      gotoxy (xpos+xmin+length-2, ypos+ymin-1);
      print (fc::BlackRightPointingPointer); // ►
      if ( isMonochron() )
        setReverse(false);
    }
  }
}