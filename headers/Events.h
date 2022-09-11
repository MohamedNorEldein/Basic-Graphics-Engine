#pragma once

#include "mndWindow.h"
#include <string>
#include <bitset>
#include "SingleLinkedList.h"

#define BITVALUE(x)  1<<x
/*
 * this is a blocking event Manger which work as follow:
 * each event is being received then handled completely before any other event can take place
 * this system dont allow parallel computing
 * but save space
 */
typedef unsigned char keyCode;

class KeyBoardMessage {
public:
    std::bitset<256> keyState;
    std::wstring string;
    int a;
private:
    // windows Api functions
    // char
private:
    void pushCharMsg(WPARAM wparam);

    void clearCharBuffer();

    // keys
    void pushKeyDownMsg(WPARAM wparam);

    void pushKeyUpMsg(WPARAM wparam);

    void clear();


public:
    // client functions
    // chars


    wchar_t popChar();

    bool bufferIsEmpty();

    size_t bufferLength();

    const wchar_t *getCharBuffer();

    // keys
    bool isDown(keyCode Key);

    bool keyEmpty();

    void clearKey();

public:
    //constructor
    inline KeyBoardMessage() :string(),keyState() {
        a = 0;
     
    };

    friend class Window;

};


struct  MouseMassege {
    int _x, _y, wheelMove;
    unsigned int type,state;

};

class MouseEvents {
    unsigned long type, state;

    int x, y,wheelMove;
public:
    MouseEvents() = default;

    ~MouseEvents() = default;

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
    unsigned long getType() {
        return type;
    }
    unsigned long getState() {
        return state;
    }
    int getWheelMove() {
        return wheelMove/120;
    }
    void handeled() {
        type = { 0 };
        state = 0;
        wheelMove = 0;

    }

private:

    void onWheelPress(int x, int y, unsigned int s);

    void onWheelRelease(int x, int y);

    void onMove(int x, int y);

    void onRightPress(int x, int y,unsigned int s );

    void onLeftPress(int x, int y, unsigned int s );

    void onRightRelease(int x, int y);

    void onLeftRelease(int x, int y);

    void onWheelMove(int x, int y, int _wheel_move);

public:
   

private:
    friend class Window;
};
