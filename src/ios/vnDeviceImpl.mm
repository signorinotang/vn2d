//
//  vnDeviceImpl.mm
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnDeviceImpl.h"
#include "vnFontImpl.h"

_vn_begin

u32 * loadPngFile(DataStream &ds, vector2i &size, vector2i &original);

DeviceImpl::DeviceImpl(ApplicationImpl *appImpl)
: m_appImpl(appImpl)
, m_program(0)
, m_vertexBuffer(0)
, m_indexBuffer(0)
, m_projMatLocation(0)
, m_viewMatLocation(0)
, m_vertices(vnmalloc(Vertex, kMaxNumVertices))
, m_indices(vnmalloc(u16, kMaxNumIndices))
, m_numVertices(0)
, m_numIndices(0)
, m_scaleInPixel(1, 1)
, m_scaleInPoint(1, 1)
, m_blendMode(kAlphaBlend) {
    
}

DeviceImpl::~DeviceImpl() {
    vnfree(m_vertices);
    vnfree(m_indices);
}

const vector2f & DeviceImpl::scaleInPoint() const {
    return m_scaleInPoint;
}

const vector2f & DeviceImpl::scaleInPixel() const {
    return m_scaleInPixel;
}

const vector2f & DeviceImpl::size() const {
    return m_size;
}

bool DeviceImpl::begin() {
    return true;
}

void DeviceImpl::end() {
    _flush();
}

void DeviceImpl::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void DeviceImpl::setTexture(const TexturePtr &texture) {
    TextureImpl *tex = (TextureImpl *)texture.ptr();
    if (tex == m_currentTexture.ptr()) {
        return ;
    }
    _flush();
    if (tex) {
        tex->grab();
    }
    m_currentTexture = tex;
}

void DeviceImpl::setBlendMode(BlendMode mode) {
    if (m_blendMode == mode) {
        return ;
    }
    _flush();
    m_blendMode = mode;
    _applyBlendMode();
}

void DeviceImpl::setViewTransform(const matrix3 &mat) {
    _flush();
    GLfloat m[16] = {
		mat.m11, mat.m12, 0, mat.m13,
		mat.m21, mat.m22, 0, mat.m23,
		0, 0, 0, 0,
		mat.m31, mat.m32, 0, mat.m33
	};
	glUniformMatrix4fv(m_viewMatLocation, 1, GL_FALSE, m);
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
	memcpy(v, vertices, sizeof(Vertex) * numVertices);
	for (u32 i = 0; i < numIndices; ++i) {
		*idx++ = *indices++ + m_numVertices;
	}
	m_numIndices += numIndices;
	m_numVertices += numVertices;
}

TexturePtr DeviceImpl::createTexture(const vector2i &size) {
    TextureImpl *tex = vnnew TextureImpl(size);
    if (!tex->_init(0)) {
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
    if (!tex->_init(data)) {
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
    if (!font->_create(size, m_scaleInPixel.y)) {
        delete font;
        return FontPtr::kNull;
    }
    return font;
}

bool DeviceImpl::_init(const vector2i &size, const vector2i &sizeInPoint, const vector2i &sizeInPixel) {
    if (size.x <= 0 || size.y <= 0) {
        return false;
    }
    m_size = size;
    m_scaleInPoint = vector2f(sizeInPoint) / m_size;
    m_scaleInPixel = vector2f(sizeInPixel) / m_size;
    
    glViewport(0, 0, sizeInPixel.x, sizeInPixel.y);
    _setupGLES();
    return true;
}

void DeviceImpl::_setupGLES() {
    glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
    glClearColor(0, 0, 0, 0);
	
	const c8 *vsShader =
    "attribute vec4 position;"
    "attribute vec4 color;"
    "attribute vec2 texcoord;"
    "uniform mat4 projMat;"
    "uniform mat4 viewMat;"
    "varying vec4 v_color;"
    "varying vec2 v_texcoord;"
    "void main() {"
    "	gl_Position = projMat * (viewMat * position);"
    "	v_color = color;"
    "	v_texcoord = texcoord;"
    "}"
    "";
	
	const c8 *psShader =
    "precision lowp float;"
    "uniform sampler2D image;"
    "varying vec4 v_color;"
    "varying vec2 v_texcoord;"
    "void main() {"
    "	gl_FragColor = v_color * texture2D(image, v_texcoord);"
    "}"
    "";
	
	GLint status = 0;
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsShader, 0);
	glCompileShader(vs);
	
	GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ps, 1, &psShader, 0);
	glCompileShader(ps);
	
	m_program = glCreateProgram();
	glAttachShader(m_program, vs);
	glAttachShader(m_program, ps);
	
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "color");
	glBindAttribLocation(m_program, 2, "texcoord");
	
	glLinkProgram(m_program);
    
	
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if ( status == GL_FALSE ) {
		glDeleteProgram(m_program);
	}
	
	glDeleteShader(vs);
	glDeleteShader(ps);
    
	m_projMatLocation = glGetUniformLocation(m_program, "projMat");
	m_viewMatLocation = glGetUniformLocation(m_program, "viewMat");
    
	glUseProgram(m_program);
	
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)8);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)12);
	
	GLfloat m[4][4];
	memset(m, 0, sizeof(GLfloat) * 16);
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
	
	glUniformMatrix4fv(m_viewMatLocation, 1, GL_FALSE, m[0]);
	
	m[0][0] = 2.f / m_size.x;
	m[1][1] = -2.f / m_size.y;
	m[2][2] = 0;
	m[3][0] = -1;
	m[3][1] = 1;
	m[3][2] = 1;
	
	glUniformMatrix4fv(m_projMatLocation, 1, GL_FALSE, m[0]);
    
    m_nullTexture = vnnew TextureImpl(vector2i(1, 1));
    m_nullTexture->_init(&color_t::kWhite);
}

void DeviceImpl::_flush() {
	if (m_numVertices) {
		// bind texture ..
		if (m_currentTexture.null()) {
			m_nullTexture->_bind();
		} else {
			m_currentTexture->_bind();
		}
		// draw ..
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), m_vertices, GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(u16), m_indices, GL_STREAM_DRAW);
		glDrawElements(GL_TRIANGLES, (int)m_numIndices, GL_UNSIGNED_SHORT, 0);
		m_numIndices = 0;
		m_numVertices = 0;
	}
}

void DeviceImpl::_applyBlendMode() {
	switch (m_blendMode) {
		case kAlphaBlend:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
			
		case kAlphaAdditive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
	}
}

_vn_end