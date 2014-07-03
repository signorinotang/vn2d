#ifndef __vn2d__vnDeviceImpl__
#define __vn2d__vnDeviceImpl__


#include <d3d9.h>
#include <d3dx9.h>
#include "vnDevice.h"
#include "vnTextureImpl.h"
#include "vnApplicationImpl.h"
#include "vnFilePath.h"

_vn_begin

class DeviceImpl : public Device {
public:
	DeviceImpl(ApplicationImpl* appImpl);
	~DeviceImpl();

	virtual const vector2f & scaleInPoint() const;
	virtual const vector2f & scaleInPixel() const;
	virtual const vector2f & size() const;

	virtual bool begin();
	virtual void end();
	virtual void clear();
	
	virtual void setTexture(const TexturePtr& texture);
	virtual void setBlendMode(BlendMode mode);
	virtual void setViewTransform(const matrix3& mat);

	virtual void drawTriangles(size_t count, const Vertex *vertices, size_t numVertices, u16 *indices);
	virtual TexturePtr createTexture(const vector2i &size);
	virtual TexturePtr loadTexture(DataStream &ds, vector2i *original = 0);

	virtual FontPtr createFont(u32 size);
	
	//size logic coordinate system
	//sizeInPoint window coordinate system
	//sizeInPixel pixel coordinate system
	bool _init(HWND window, const vector2i& size, const vector2i& sizeInPoint, const vector2i &sizeInPixel, const FilePath &fontFile);
	void buildProjectionMatrix(u32 width, u32 height, D3DXMATRIX &matrix);

	void applyRenderStates();

	void _present();
	bool _isDeviceLost();
	void _resetDevice();

protected:
	void _applyBlendMode();
	void _flush();


	enum {
		kMaxNumVertices = 2048,
		kMaxNumIndices = 3072,
	};

	IDirect3DVertexShader9*		 m_vertexShader;
	IDirect3DPixelShader9*		 m_pixelShader;

	ID3DXConstantTable*          m_vsConstants;
	
	IDirect3DDevice9*			 m_device;
	D3DXHANDLE					 m_hProjMat;
	D3DXHANDLE					 m_hViewMat;

	D3DXMATRIX					 m_projectionMatrix;
	D3DXMATRIX					 m_viewMatrix;

	Device::BlendMode			 m_blendMode;

	D3DPRESENT_PARAMETERS		 m_deviceParameters;
	vector2f m_scaleInPoint;
	vector2f m_scaleInPixel;
	vector2f m_size;

	u32			m_numVertices;
	u32			m_numIndices;
	Vertex *	m_vertices;
	u16 *		m_indices;
	IDirect3DVertexDeclaration9* m_vertexFormat;

	RefCountedPtr<TextureImpl> m_nullTexture, m_currentTexture;

	ApplicationImpl *m_appImpl;

	FilePath m_fontFile;
	
};

_vn_end

#endif