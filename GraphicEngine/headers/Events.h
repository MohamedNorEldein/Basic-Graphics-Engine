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

class KeyBoardEvent {
public:
    std::bitset<256> keyState;
    Array<wchar_t,128> string;
    int a;
    UINT type;
private:
    // windows Api functions
    // char
private:
    void pushCharMsg(WPARAM wparam, UINT msg);

    void clearCharBuffer();

    // keys
    void pushKeyDownMsg(WPARAM wparam, UINT msg);

    void pushKeyUpMsg(WPARAM wparam, UINT msg);

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
    inline KeyBoardEvent() :string(),keyState() {
        a = 0;
     
    };

    friend class Window;

};


struct  MouseMassege {
    int _x, _y, wheelMove;
    unsigned int type,state;

};

class MouseEvents {
    unsigned int type, state;

    int x, y,wheelMove;
    int dx, dy;
    bool rawInputEnable;
   
public:
    MouseEvents() = default;

    ~MouseEvents() = default;

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    int get_dx() const {
        return dx;
    }

    int get_dy() const {
        return dy;
    }

    bool IsRawInputEnable() {
        return rawInputEnable;
    }

     int getType() {
        return type;
    }
     int getState() {
        return state;
    }
    int getWheelMove() {
        int a = wheelMove/120;
        wheelMove = 0;
        return a;
    }
    void handeled() {
        type = { 0 };
        state = 0;
        wheelMove = 0;

    }

    void EnableRawMouse();

    void DisableRawMouse();

private:
    

    void onWheelPress(int x, int y, unsigned int s);

    void onWheelRelease(int x, int y, unsigned int s);

    void onMove(int x, int y, unsigned int s);

    void onRightPress(int x, int y,unsigned int s );

    void onLeftPress(int x, int y, unsigned int s );

    void onRightRelease(int x, int y, unsigned int s);

    void onLeftRelease(int x, int y, unsigned int s);

    void onWheelMove(int x, int y, int _wheel_move, unsigned int s);

public:
   

private:
    friend class Window;
};
