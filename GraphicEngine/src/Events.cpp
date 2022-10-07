
#include "Events.h"
// imgui
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


void KeyBoardEvent::pushCharMsg(WPARAM wparam, UINT msg) {
    if (wparam == VK_BACK) {
        string.pop_head();
    }
    type = msg;
    try {
        string.push_head(wchar_t(wparam));
    }
    catch (ArrayIsFilled&) {
        string.pop_tail();
        string.push_head(wchar_t(wparam));

    }
}

void KeyBoardEvent::clearCharBuffer() {
    string.clear();
}

void KeyBoardEvent::pushKeyDownMsg(WPARAM wparam, UINT msg) {
    keyState[wparam] = true;
    type = msg;

}

void KeyBoardEvent::pushKeyUpMsg(WPARAM wparam, UINT msg) {
    keyState[wparam] = 0;
    type = msg;

}

void KeyBoardEvent::clear() {
    string.clear();
    keyState = 0;

}

wchar_t KeyBoardEvent::popChar() {
    
    return string.pop_head();
}

bool KeyBoardEvent::bufferIsEmpty() {
    return string.empty();
}

size_t KeyBoardEvent::bufferLength() {
    return string.length();
}

bool KeyBoardEvent::isDown(keyCode key) {
 
    //printf("%d", bool(keyState[key]));
    if (bool(keyState[key])) {

        return true;
    }
    return false;
}

bool KeyBoardEvent::keyEmpty() {
    if (keyState.count())
        return false;
    return true;
}

void KeyBoardEvent::clearKey() {
    keyState = 0;
}


const wchar_t *KeyBoardEvent::getCharBuffer() {
    return string.get();
}

//------------------------------------------------------

void MouseEvents::EnableRawMouse()
{
    rawInputEnable = true;
}

void MouseEvents::DisableRawMouse()
{
    rawInputEnable = false;
}

void MouseEvents::onWheelPress(int _x, int _y,unsigned int s)
{
    dx = _x - x;
    dy = _y - y;

    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_MBUTTONDOWN;
    state = s;

}

void MouseEvents::onWheelRelease(int _x, int _y,unsigned int s) {
   
    dx = _x - x;
    dy = _y - y;

    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_MBUTTONUP;
    state = s;
}

void MouseEvents::onMove(int _x, int _y , unsigned int s) {
   
    dx = _x-x;
    dy = _y-y;

    x = _x;
    y = _y;
    wheelMove = 0;
    type = 0;
    state = s;
}

void MouseEvents::onRightPress(int _x, int _y, unsigned int s ) {

    dx = _x - x;
    dy = _y - y;

    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_RBUTTONDOWN;
    state = s;

}

void MouseEvents::onLeftPress(int _x, int _y, unsigned int s ) {
 
    dx = _x - x;
    dy = _y - y;

    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_LBUTTONDOWN;
    state = s;

}

void MouseEvents::onRightRelease(int _x, int _y, unsigned int s) {
 
    dx = _x - x;
    dy = _y - y;
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_RBUTTONUP;
    state = s;

}
void MouseEvents::onLeftRelease(int _x, int _y, unsigned int s) {
    dx = _x - x;
    dy = _y - y;
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_LBUTTONUP;
    state = s;

}

void MouseEvents::onWheelMove(int _x, int _y, int _wheel_move, unsigned int s) {
    dx = _x - x;
    dy = _y - y;
    x = _x;
    y = _y;
    wheelMove = _wheel_move;
    type = WM_MOUSEWHEEL;
    state = s;

}

