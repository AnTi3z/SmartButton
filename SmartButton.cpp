#include <SmartButton.h>

// SmartButton

// Private
void SmartButton::DoAction(input in) {
  switch (in) {
    case input::Release:
      //Serial.println("Release");
      switch (btState) {
        case state::Click:
          btState=state::Idle;
          offClick();
          break;
        case state::Hold:
          btState=state::Idle;
          offHold(); 
          break;
        case state::LongHold:
          btState=state::Idle;
          offLongHold(); 
          break;
        case state::ForcedIdle:
          btState=state::Idle;
          onIdle(); 
          break;
      }
      break;
    case input::WaitDebounce:
      switch (btState) {
        case state::PreClick:
          //Serial.println("Click");
          btState=state::Click;
          onClick();
          break;
      }
      break;
    case input::WaitHold:
      switch (btState) {
        case state::Click:
          //Serial.println("Hold");
          btState=state::Hold;
          onHold();
          break;
      }
      break;
    case input::WaitLongHold:
      switch (btState) {
        case state::Hold:
          //Serial.println("Long");
          btState=state::LongHold;
          onLongHold();
          break;
      }
      break;
    case input::WaitIdle:
      switch (btState) {
        case state::LongHold:
          //Serial.println("ForcedIdle");
          btState=state::ForcedIdle;
          break;
      }
      break;
    case input::Press:
      switch (btState) {
        case state::Idle:
          //Serial.println("Press");
          pressTimeStamp=0xFFFF & millis();
          btState=state::PreClick;
          break;
      }
      break;
  }
}

// Public
void SmartButton::run() {
  if (!digitalRead(btPin))  {
    //if (btState == state::Idle) {
      DoAction(input::Press);
    //} else {
      auto elapsed = static_cast<unsigned int>(0xFFFF & millis()) - pressTimeStamp;
      if (elapsed > SmartButton_debounce) DoAction(input::WaitDebounce);
      if (elapsed > SmartButton_hold) DoAction(input::WaitHold);
      if (elapsed > SmartButton_long) DoAction(input::WaitLongHold);
      if (elapsed > SmartButton_idle) DoAction(input::WaitIdle);
    //}
  } else {
    DoAction(input::Release);
  }
}

