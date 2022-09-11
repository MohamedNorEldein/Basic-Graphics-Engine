
#include<Graphics.h>
#include <Vector>


#ifndef BINDABLE
#define BINDABLE


class Bindable
{
public:
	virtual	void bind(Graphics& gfx) = 0;
	virtual ~Bindable() {
		cout("bindable deleted");
	};

public:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) {return gfx.pcontext;}
	
	static ID3D11Device* GetDevice(Graphics& gfx) {return gfx.pdevice;}

};


#endif // !BINDABLE