#include "vnApplicationImpl.h"
#include "vnTreeDataParser.h"
#include "vnLog.h"
#include "vnDeviceImpl.h"
#include "vnUnicode.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Imm32.lib")
_vn_begin
ApplicationImpl* ApplicationImpl::m_self = 0;
static bool ldown = false;

ApplicationImpl::ApplicationImpl()
: m_type(Application::kType_iPad)
, m_scale(1)
, m_viewSize(1024, 768)
, m_delegate(0)
, m_device(0)
, m_deltaTime(0.f)
, m_delay(0)
, m_isInputAsciiOnly(true)
, m_interval(0.0166666666666f){
	m_self = this;
}

ApplicationImpl::~ApplicationImpl() {
	VN_SAFE_DELETE(m_device);
}

int ApplicationImpl::main(Application::Delegate* delegate, bool horizontal) {
	vnassert(delegate);
	m_delegate = delegate;

	_init(horizontal);	
	
	_createView();      //create window

	m_delegate->onInitialize();

	_windowRun();

	return 0;
}

Application::Type ApplicationImpl::type() {
	return m_type;
}

f32 ApplicationImpl::scale() {
	return m_scale;
}

const vector2i & ApplicationImpl::viewSize() {
	return m_viewSize;
}

const str8 & ApplicationImpl::resourcePath() {
	return m_resourcePath;
}

const str8 & ApplicationImpl::cachePath() {
	return m_cachePath;
}

const str8 & ApplicationImpl::tempPath() {
	return m_tempPath;
}

const str8 & ApplicationImpl::documentPath() {
	return m_documentPath;
}

Device * ApplicationImpl::createDevice(const vector2f &size) {
	m_device = new DeviceImpl(this);
	if (!m_device->_init(m_wnd, size, m_viewSize, m_viewSize * m_scale, m_fontFileName.c_str())) {
		delete m_device;
		m_device = 0;
	}
	return m_device;
}



void ApplicationImpl::_init(bool horizontal){
	TreeDataNode* node = TreeDataParser::instance().parse("0:/config.tdl");
	if (!node) {
		VN_LOG_ERROR("open 0:/config.tdl failed !!!");
		return;
	}
	str8 result;
	vector2i size;
	node->queryAttributeString("device", result);
	node->queryAttributeString("font", m_fontFileName);
	node->queryAttributeString("res_path", m_resourcePath);
	node->queryAttributeString("cache_path", m_cachePath);
	node->queryAttributeString("temp_path", m_tempPath);
	node->queryAttributeString("document_path", m_documentPath);
	node->queryAttributeString("title", m_title);
	node->drop();

	if (result == "iphone_4") {
		size.y = 568;
		size.x = 320;
		m_scale = 2;
		m_type = Application::kType_iPhone_4;
	}
	else if (result == "iphone_3_5") {
		size.y = 480;
		size.x = 320;
		m_scale = 2;
		m_type = Application::kType_iPhone_3_5;
	}
	else if (result == "ipad") {
		size.y = 1024;
		size.x = 768;
		m_scale = 1;
		m_type = Application::kType_iPad;
	}
	if (!horizontal) {
		m_viewSize = size;
	}
	else {
		m_viewSize.x = size.y;
		m_viewSize.y = size.x;
	}

	
}

void ApplicationImpl::_view_update(vn::f32 deltaTime) {
	m_delegate->onUpdate(deltaTime);
}

void ApplicationImpl::_view_render() {
	m_delegate->onRender(m_device);
}
void ApplicationImpl::_view_finalize() {
	m_delegate->onFinalize();
}

void ApplicationImpl::_view_touchBegan(long tid, const vector2f &pt) {
	m_delegate->onTouchBegan(1, pt);
}

void ApplicationImpl::_view_touchMoved(long tid, const vector2f &pt) {
	m_delegate->onTouchMoved(1, pt);
}

void ApplicationImpl::_view_touchEnded(long tid, const vector2f &pt) {
	m_delegate->onTouchEnded(1, pt);
}

void ApplicationImpl::setInputAsciiOnly(bool flag) {
	if (!flag)
	{
		::ImmAssociateContext(m_wnd, m_hIMC);
		m_isInputAsciiOnly = false;
	}
	else
	{
		m_hIMC = ::ImmAssociateContext(m_wnd, NULL);
		m_isInputAsciiOnly = true;
	}
}

bool ApplicationImpl::isInputAsciiOnly() {
	return m_isInputAsciiOnly;
}


void ApplicationImpl::_createView() {

	WNDCLASSW wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = windowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ::GetModuleHandleW(NULL);
	wc.hIcon = LoadIconW(0, IDI_APPLICATION);
	wc.hCursor = LoadCursorW(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"game";

	if (!::RegisterClass(&wc)) {
		VN_LOG_ERROR("window register filed!!!");
	}
	
	//DWORD wndStyle = WS_OVERLAPPEDWINDOW; WS_MINIMIZEBOX
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ;

	RECT r = { 0, 0, (LONG)m_viewSize.x * m_scale, (LONG)m_viewSize.y * m_scale };
	::AdjustWindowRect(&r, wndStyle, FALSE);
	r.right -= r.left;
	r.bottom -= r.top;

	strw title;
	Unicode::convert(m_title.c_str(), m_title.length(), title);

	m_wnd = ::CreateWindowW(
		wc.lpszClassName
		, title.c_str()
		, wndStyle
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, r.right
		, r.bottom
		, 0
		, 0
		, wc.hInstance
		, 0);
	if (!m_wnd) {
		VN_LOG_ERROR("Create window failed !!!")
	}

	ShowWindow(m_wnd, SW_NORMAL);

	m_hIMC = ::ImmAssociateContext(m_wnd, NULL);
}


void ApplicationImpl::_windowRun() {
	::timeBeginPeriod(1);
	u32 oTime = ::timeGetTime();
	u32 nTime = 0;

	for (;;) {
		if (::PeekMessageW(&m_msg, NULL, 0, 0, PM_REMOVE)) {
			if (m_msg.message == WM_QUIT) {
				_windowQuit();
				break;
			}
			::TranslateMessage(&m_msg);
			::DispatchMessageW(&m_msg);
		}
		else {
			u32 nTime = ::timeGetTime();
			m_deltaTime = (nTime - oTime) / 1000.f;
			m_delay = m_deltaTime;
			if (m_delay > m_interval)
			{
				oTime = nTime;
				//thread_lock();
				_view_update(m_deltaTime);
				if (m_device) {
					if (!m_device->_isDeviceLost()) {
						_view_render();
						m_device->_present();
					}
				}
				//thread_unlock();
			}
			else
			{
				::Sleep(1);
			}
		}
	}
}

void  ApplicationImpl::_windowQuit() {
	m_delegate->onFinalize();
	if (m_wnd) {
		::DestroyWindow(m_wnd);
		m_wnd = 0;
	}
}

LRESULT CALLBACK ApplicationImpl::windowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg)
	{
	case WM_PAINT:
		//m_self->thread_lock();
		//m_self->m_delegate->draw();
		//m_self->thread_unlock();
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		//width = LOWORD(lparam);
		//height = HIWORD(lparam);
		//m_self->m_delegate->resize(width, height);
		break;
	case WM_CAPTURECHANGED:
		if (ldown == true){
			m_self->m_delegate->onTouchCancelled(1, vn::vector2f(0, 0));
			::ReleaseCapture();
			ldown = false;
		}
		break;

	default:
		break;
	}
	if(msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST || msg == WM_MOUSELEAVE) {
		onMouseMessage(wnd, msg, wparam, lparam);
	}
	else if (msg >= WM_KEYFIRST && msg <= WM_KEYLAST) {
		onKeyboardMessage(wnd, msg, wparam, lparam);
	}
	return DefWindowProc(wnd, msg, wparam, lparam);
}

void ApplicationImpl::onMouseMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static s32 capture = 0;
	
	//static bool rdown = false;
	static bool leave = false;

	switch (msg)
	{
		case WM_MOUSELEAVE:
		{
			if (!leave)
			{
				leave = true;
			}
		}
			break;
		case WM_LBUTTONDOWN:
		{
			//if (capture++ == 0)
			{
				::SetCapture(wnd);
			}
			ldown = true;
			m_self->_view_touchBegan(1, vector2f((short)LOWORD(lparam) / m_self->m_scale, (short)HIWORD(lparam) / m_self->m_scale));
		}
			break;
		case WM_LBUTTONUP:
		{

			if (ldown)
			{
				ldown = false;
				//if (--capture == 0)
				{
					::ReleaseCapture();
				}
				m_self->_view_touchEnded(1, vector2f((short)LOWORD(lparam) / m_self->m_scale, (short)HIWORD(lparam) / m_self->m_scale));
			}
		}
			break;
		case WM_MOUSEMOVE:
		{
			/*if (leave)
			{
				leave = false;
				::TrackMouseEvent(&(m_self->m_track));
			}*/
			if (ldown) {
				m_self->_view_touchMoved(1, vector2f((short)LOWORD(lparam) / m_self->m_scale, (short)HIWORD(lparam) / m_self->m_scale));
			}
		
		}
		break;
	}
}


void ApplicationImpl::onKeyboardMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {

		case WM_CHAR:
		{
			if (wparam == '\b')
			{
				m_self->m_delegate->onDeleteBackward();
			}
			else
			{
				str32 text;
				text.push_back((c32)wparam);
				m_self->m_delegate->onInsertText	(text);
			}
		}
		break;
	}
}



_vn_end