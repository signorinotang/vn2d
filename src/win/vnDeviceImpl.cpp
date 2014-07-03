#include "vnDeviceImpl.h"
#include "vnFileSystem.h"
#include "vnDataStream.h"
#include "vnFontImpl.h"
#include "vnTreeDataParser.h"
#include "vnUnicode.h"
#include <iostream>

_vn_begin

static const c8 s_shader[] =
"uniform float4x4 projMat;"
"uniform float4x4 viewMat;"
"sampler image : register(s0);"
"void vsMain( in float4 iPos : POSITION,"
"			  in float4 iColor : COLOR,"
"			  in float2 iTex : TEXCOORD0,"
"			  out float4 oPos : POSITION,"
"			  out float4 oColor : COLOR,"
"			  out float2 oTex : TEXCOORD0 ) {"
"	oPos = mul( mul( iPos, viewMat ), projMat );"
"	oColor = iColor;"
"	oTex = iTex;"
"}"
"void psMain( in float4 iColor : COLOR,"
"			  in float2 iTex : TEXCOORD0,"
"			  out float4 oColor : COLOR0 ) {"
"	oColor = iColor * tex2D( image, iTex );"
"}"
;

static const u32 s_shaderLength = sizeof(s_shader)-1;

u32 * loadPngFile(DataStream &ds, vector2i &size, vector2i &original);
Texture* createTextureFromPNG(DeviceImpl* device, DataStream* ds, u32& width, u32& height);

bool DeviceImpl::_init(HWND window, const vector2i& size, const vector2i& sizeInPoint, const vector2i &sizeInPixel, const FilePath &fontFile) {
	m_fontFile = fontFile;
	m_size = size;
	m_scaleInPoint = vector2f(sizeInPoint) / m_size;
	m_scaleInPixel = vector2f(sizeInPixel) / m_size;

	IDirect3D9 *d3d9 = ::Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		printf("d3d9 init failed");  
		return false;
	}
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	DWORD devBehaviorFlags = D3DCREATE_MULTITHREADED;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	m_deviceParameters.BackBufferWidth = (UINT)sizeInPixel.x;
	m_deviceParameters.BackBufferHeight = (UINT)sizeInPixel.y;
	m_deviceParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_deviceParameters.BackBufferCount = 1;
	m_deviceParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_deviceParameters.MultiSampleQuality = 0;
	m_deviceParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_deviceParameters.hDeviceWindow = window;
	m_deviceParameters.Windowed = TRUE;
	m_deviceParameters.EnableAutoDepthStencil = FALSE;
	m_deviceParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	m_deviceParameters.Flags = 0;
	m_deviceParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_deviceParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,		// primary adapter
		D3DDEVTYPE_HAL,         // device type
		window,					// window associated with device
		devBehaviorFlags,		// 
		&m_deviceParameters,    // present parameters
		&m_device			    // return created device
		);
	if (FAILED(hr))
	{
		d3d9->Release();
		return false;
	}
	d3d9->Release();

	if (m_device == 0)
	{
		printf("device create failed");
		return false;
	}
	ID3DXBuffer* sbuf = 0;
	ID3DXBuffer* errorMsg = 0;
	D3DXCompileShader(s_shader, s_shaderLength, NULL, NULL, "vsMain", "vs_2_0", 0, &sbuf, &errorMsg, &m_vsConstants);
	if (errorMsg) {
#ifdef _DEBUG
		std::cout << (const char *)errorMsg->GetBufferPointer() << std::endl;
#endif
		errorMsg->Release();
	}
	if (!sbuf) {
		m_device->Release();
		m_device = 0;
		return false;
	}

	m_device->CreateVertexShader((DWORD *)sbuf->GetBufferPointer(), &m_vertexShader);
	sbuf->Release();
	D3DXCompileShader(s_shader, s_shaderLength, NULL, NULL, "psMain", "ps_2_0", 0, &sbuf, NULL, NULL);
	if (errorMsg) {
#ifdef _DEBUG
		std::cout << (const char *)errorMsg->GetBufferPointer() << std::endl;
#endif
		errorMsg->Release();
	}
	if (!sbuf) {
		m_vertexShader->Release();
		m_vertexShader = 0;
		m_device->Release();
		m_device = 0;
		return false;
	}
	m_device->CreatePixelShader((DWORD *)sbuf->GetBufferPointer(), &m_pixelShader);
	sbuf->Release();

	m_hProjMat = m_vsConstants->GetConstantByName(0, "projMat");
	m_hViewMat = m_vsConstants->GetConstantByName(0, "viewMat");

	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION },
		{ 0, 8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD },
		D3DDECL_END()
	};


	hr = m_device->CreateVertexDeclaration(elements, &m_vertexFormat);


	m_nullTexture = vnnew TextureImpl(vector2i(1, 1));
	m_nullTexture->createTexture(m_device);
	D3DLOCKED_RECT ret;
	m_nullTexture->m_texture->LockRect(0, &ret, NULL, 0);
	*(u32 *)ret.pBits = 0xFFFFFFFF;
	m_nullTexture->m_texture->UnlockRect(0);


	m_vertices = vnmalloc(Vertex, kMaxNumVertices);
	m_indices = vnmalloc(u16, kMaxNumIndices);


	buildProjectionMatrix(size.x, size.y, m_projectionMatrix);
	applyRenderStates();

	return true;


}



DeviceImpl::DeviceImpl(ApplicationImpl* appImpl)
: m_appImpl(appImpl)
, m_scaleInPixel(1,1)
, m_scaleInPoint(1,1)
, m_vertices(0)
, m_indices(0)
, m_vertexShader(0)
, m_pixelShader(0)
, m_device(0)
, m_vertexFormat(0)
, m_hProjMat(0)
, m_hViewMat(0)
, m_nullTexture(0)
, m_currentTexture()
, m_numIndices(0)
, m_numVertices(0){
	::D3DXMatrixIdentity(&m_projectionMatrix);
	::D3DXMatrixIdentity(&m_viewMatrix);
	FontImpl::ftInit();
}

DeviceImpl::~DeviceImpl() {

	//delete
	if (m_vertices)
	{
		vnfree(m_vertices);
	}
	if (m_indices)
	{
		vnfree(m_indices);
	}
	
	FontImpl::ftDone();
}

const vector2f & DeviceImpl::scaleInPoint() const {
	return m_scaleInPoint;
}

const vector2f & DeviceImpl::scaleInPixel() const {
	return m_scaleInPixel;
}

bool DeviceImpl::begin()  {
	HRESULT hr = m_device->BeginScene();
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

void DeviceImpl::clear() {
	HRESULT hr = m_device->Clear(0, 0, D3DCLEAR_TARGET, (D3DCOLOR)(0x00000000), 1.0f, 0);
}

void DeviceImpl::end() {
	_flush();
	m_device->EndScene();
}

void DeviceImpl::setTexture(const TexturePtr& texture){
	TextureImpl* tex = dynamic_cast<TextureImpl *>(texture.ptr());
	if (!tex) {
		tex = m_nullTexture.ptr();
	}
	if (tex != m_currentTexture.ptr()) {
		_flush();
		tex->grab();
		m_currentTexture = tex;
	}
}

void DeviceImpl::setBlendMode(BlendMode mode) {
	if (mode != m_blendMode) {
		_flush();
		m_blendMode = mode;
		_applyBlendMode();
	}
}

void DeviceImpl::setViewTransform(const matrix3& mat) {
	_flush();
	float m[16] = {
		mat.m11, mat.m12, 0, mat.m13,
		mat.m21, mat.m22, 0, mat.m23,
		0, 0, 0, 0,
		mat.m31, mat.m32, 0, mat.m33
	};
	m_vsConstants->SetMatrix(m_device, m_hViewMat, (D3DXMATRIX*)m);
}

void DeviceImpl::drawTriangles(size_t count, const Vertex *vertices, size_t numVertices, u16 *indices) {
	size_t numIndices = count * 3;
	size_t totalIndices = m_numIndices + numIndices;
	size_t totalVertices = m_numVertices + numVertices;
	if (totalIndices > kMaxNumIndices || totalVertices > kMaxNumVertices) {
		_flush();
	}
	Vertex *v = m_vertices + m_numVertices;
	u16 *idx = m_indices + m_numIndices;
	
	for (u32 i = 0; i < numVertices; ++i, ++v, ++vertices) {
		v->position = vector2f(vector2i(vertices->position * m_scaleInPixel)) / m_scaleInPixel;
		v->position = vertices->position;
		v->color = ((vertices->color).convertToBGRA());
		v->texcoord = vertices->texcoord;
	}
	
	//memcpy(v, vertices, sizeof(Vertex)* numVertices);

	for (u32 i = 0; i < numIndices; ++i) {
		*idx++ = *indices++ + m_numVertices;
	}
	m_numIndices += numIndices;
	m_numVertices += numVertices;
}

void DeviceImpl::buildProjectionMatrix(u32 width, u32 height, D3DXMATRIX &matrix) {

	matrix._11 = 2.f / width;
	matrix._22 = -2.f / height;
	matrix._33 = 0;
	matrix._41 = -1 + (1 / (width * m_scaleInPixel.x));
	matrix._42 = 1 + (1 / (height * m_scaleInPixel.y));
	matrix._43 = 0;

	m_vsConstants->SetMatrix(m_device, m_hProjMat, (D3DXMATRIX*)&matrix);
}

TexturePtr DeviceImpl::createTexture(const vector2i &size) {
	
	TextureImpl *tex = vnnew TextureImpl(size);
	if (!tex->_init(m_device, 0)) {
		delete tex;
		return TexturePtr();
	}
	return tex;
}

TexturePtr DeviceImpl::loadTexture(DataStream &ds, vector2i *original) {

	vector2i size, _original;
	u32 *data = loadPngFile(ds, size, _original);
	if (!data) {
		return TexturePtr();
	}
	TextureImpl *tex = vnnew TextureImpl(size);
	if (!tex->_init(m_device, data)) {
		delete tex;
		vnfree(data);
		return TexturePtr();
	}
	vnfree(data);
	if (original) {
		*original = _original;
	}
	return tex;
}


FontPtr DeviceImpl::createFont(u32 size) {
	FontImpl *font = vnnew FontImpl();

	if (!font->init(m_fontFile, size, m_scaleInPixel.y)) {
		delete font;
		return FontPtr::kNull;
	}
	return font;
}

void DeviceImpl::applyRenderStates() {
	m_device->SetVertexDeclaration(m_vertexFormat);
	m_device->SetVertexShader(m_vertexShader);
	m_device->SetPixelShader(m_pixelShader);

	m_vsConstants->SetMatrix(m_device, m_hProjMat, &m_projectionMatrix);
	m_vsConstants->SetMatrix(m_device, m_hViewMat, &m_viewMatrix);

	//disable Zorder
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	//disable face cull
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_currentTexture.null())
	{
		m_device->SetTexture(0, m_nullTexture->m_texture);
		m_currentTexture = m_nullTexture;
	}
	else
	{
		m_device->SetTexture(0, m_currentTexture->m_texture);
	}

	_applyBlendMode();

	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void DeviceImpl::_applyBlendMode() {
	switch (m_blendMode)
	{
	case Device::BlendMode::kAlphaBlend:
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case Device::BlendMode::kAlphaAdditive:
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	}

}

void DeviceImpl::_present() {
	m_device->Present(NULL, NULL, NULL, NULL);
}

void DeviceImpl::_flush() {
	if (!m_numIndices) {
		return;
	}
	m_device->SetTexture(0, m_currentTexture->m_texture);
	HRESULT hr = m_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_numVertices, m_numIndices / 3, m_indices, D3DFMT_INDEX16, m_vertices, sizeof(Vertex));
	if (FAILED(hr)) {
		//::DebugBreak();
	}
	m_numIndices = 0;
	m_numVertices = 0;
}

const vector2f& DeviceImpl::size() const{
	return m_size;
}


bool DeviceImpl::_isDeviceLost() {
	HRESULT hr = m_device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST) {
		::Sleep(50);
		return true;
	}
	else if (hr == D3DERR_DRIVERINTERNALERROR) {
		::PostQuitMessage(0);
		return true;
	}
	else if (hr == D3DERR_DEVICENOTRESET) {
		_resetDevice();
		return false;
	}
	return false;

}

void DeviceImpl::_resetDevice() {
	m_device->Reset(&m_deviceParameters);
	applyRenderStates();
}



_vn_end