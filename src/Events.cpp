
#include "Events.h"
// imgui
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


void KeyBoardMessage::pushCharMsg(WPARAM wparam) {
    if (wparam == VK_BACK) {

        string.pop_back();
    }
    string += (wchar_t(wparam));
}

void KeyBoardMessage::clearCharBuffer() {
    string.clear();
}

void KeyBoardMessage::pushKeyDownMsg(WPARAM wparam) {
    keyState[wparam] = true;
}

void KeyBoardMessage::pushKeyUpMsg(WPARAM wparam) {
    keyState[wparam] = 0;
}

void KeyBoardMessage::clear() {
    string.clear();
    keyState = 0;
}

wchar_t KeyBoardMessage::popChar() {
    wchar_t a = string[string.length()];
    string.pop_back();
    return a;
}

bool KeyBoardMessage::bufferIsEmpty() {
    return string.empty();
}

size_t KeyBoardMessage::bufferLength() {
    return string.length();
}

bool KeyBoardMessage::isDown(keyCode key) {
    if (keyState[key])
        return true;
    return false;
}

bool KeyBoardMessage::keyEmpty() {
    if (keyState.count())
        return false;
    return true;
}

void KeyBoardMessage::clearKey() {
    keyState = 0;
}


const wchar_t *KeyBoardMessage::getCharBuffer() {
    return string.c_str();
}

//------------------------------------------------------

void MouseEvents::onWheelPress(int _x, int _y,unsigned int s)
{
   
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_MBUTTONDOWN;
    state = s;

}

void MouseEvents::onWheelRelease(int _x, int _y) {
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_MBUTTONUP;
    state = 0;
}

void MouseEvents::onMove(int _x, int _y) {
    x = _x;
    y = _y;
    wheelMove = 0;
    type = 0;
    state = 0;
}

void MouseEvents::onRightPress(int _x, int _y, unsigned int s ) {
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_RBUTTONDOWN;
    state = s;

}

void MouseEvents::onLeftPress(int _x, int _y, unsigned int s ) {
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_LBUTTONDOWN;
    state = s;

}

void MouseEvents::onRightRelease(int _x, int _y) {
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_RBUTTONUP;
    state = 0;

}
void MouseEvents::onLeftRelease(int _x, int _y) {
    x = _x;
    y = _y;
    wheelMove = 0;
    type = WM_LBUTTONUP;
    state = 0;

}

void MouseEvents::onWheelMove(int _x, int _y, int _wheel_move) {
    x = _x;
    y = _y;
    wheelMove = _wheel_move;
    type = WM_MOUSEWHEEL;
    state = 0;

}

