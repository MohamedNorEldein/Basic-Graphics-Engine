#pragma once
#include "mndWindow.h"
#include "Events.h"

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
	byte rawBuffer[64];

public:
	//events
	KeyBoardEvent keyBoardEvent;
    MouseEvents mouseEvent;

public:
     void setClassName(const wchar_t* data);
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

	

private:
	inline HWND GetHandel() { return windowhandel; }

	friend class Graphics;

};


