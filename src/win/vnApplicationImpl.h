#ifndef __vn2d__vnApplicationImpl__
#define __vn2d__vnApplicationImpl__

#include "vnApplication.h"
#include "vnAllocatedObject.h"
#include <Windows.h>

_vn_begin

class DeviceImpl;
class ApplicationImpl : public vn::AllocatedObject {
public:
	ApplicationImpl();
	~ApplicationImpl();

	int main(Application::Delegate *delegate, bool horizontal);

	Application::Type type();
	f32 scale();
	const vector2i & viewSize();
	const str8 & resourcePath();
	const str8 & cachePath();
	const str8 & tempPath();
	const str8 & documentPath();

	void _windowRun();
	void _windowQuit();

	Device * createDevice(const vector2f &size);


	void _view_update(vn::f32 deltaTime);
	void _view_render();
	void _view_finalize();

	void _view_touchBegan(long tid, const vector2f &pt);
	void _view_touchMoved(long tid, const vector2f &pt);
	void _view_touchEnded(long tid, const vector2f &pt);
	//font to do
	void requireInputFocus() {}
	void releaseInputFocus() {}
	bool hasInputFocus(){ return true; }
	void setInputAsciiOnly(bool flag);
	bool isInputAsciiOnly();


protected:
	static LRESULT CALLBACK windowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void onMouseMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void onKeyboardMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	static ApplicationImpl *m_self;
	Application::Type m_type;
	f32 m_scale;
	vector2i m_viewSize;
	Application::Delegate* m_delegate;
	str8 m_resourcePath;
	str8 m_cachePath;
	str8 m_tempPath;
	str8 m_documentPath;

	str8 m_fontFileName;
	str8 m_title;
	DeviceImpl* m_device;
	
	//windows
	HWND m_wnd;
	MSG  m_msg;
	HIMC m_hIMC;
	f32  m_deltaTime;
	f32  m_delay;
	f32  m_interval;
	bool m_isInputAsciiOnly;
	//Mutex m_mutex;

private:
	void _init(bool horizontal);
	void _createView();

};


_vn_end

#endif