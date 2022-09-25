#pragma once
#include "mndWindow.h"
#include "Graphics.h"

class Window
{
private:
	//Window class
	WNDCLASSEX wc = { 0 };
	 HINSTANCE hInstance;
	//Window
	static wchar_t windowClassName[100];
	wchar_t titel[100] = { 0 };
	int width, height;
	HWND windowhandel;
	byte rawBuffer[8];
public:
	//events
	KeyBoardEvent keyBoardEvent;
    MouseEvents mouseEvent;

	int lastMouseX, lastMouseY, lastState;
public:
     void setClassName(const wchar_t* data);
private:
	// graphics
	Graphics* pgfx;

private:
    LRESULT CALLBACK EvetProtocol(HWND handel, unsigned int msg, WPARAM w, LPARAM l);

    friend LRESULT CALLBACK Protocol(HWND handel, unsigned int msg, WPARAM w, LPARAM l) ;

public:
	// constructors
    Window( const wchar_t* titel1, int width, int height);

	// Destructtors
	~Window();
public:

	// settrers
	void setTitel(const wchar_t* name);
	// getters
	wchar_t* getName();
	
	int getWidth();
	int getHeight();

	

public:
	inline Graphics& Gfx() { return *pgfx; }

};


