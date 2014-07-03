//
//  vnCompressedInt.h
//  vnlib
//
//  Created by Wayne on 14-5-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnCompressedInt_h
#define vnlib_vnCompressedInt_h

#include "vnTypes.h"

_vn_begin

class DataStream;

s64 _vn_2d_api _readCompressedInt(DataStream &stream);
void _vn_2d_api _writeCompressedInt(DataStream &stream, s64 value);

u64 _vn_2d_api _readCompressedUInt(DataStream &stream);
void _vn_2d_api _writeCompressedUInt(DataStream &stream, u64 value);

size_t _vn_2d_api _compressedDataSizeInt(s64 value);
size_t _vn_2d_api _compressedDataSizeUInt(u64 value);
size_t _vn_2d_api _compressedDataSizeLeadByte(u8 lead);

template <typename T>
struct CompressedInt;

template <typename T>
struct CompressedUInt;

template <typename _Int>
struct CompressedInt {
	_Int value;
	CompressedInt(_Int _v) : value(_v) {}
};

template <typename _Int>
struct CompressedInt<_Int &> {
	_Int &value;
	CompressedInt(_Int &_v) : value(_v) {}
};

template <typename _UInt>
struct CompressedUInt {
	_UInt value;
	CompressedUInt(_UInt _v) : value(_v) {}
};

template <typename _UInt>
struct CompressedUInt<_UInt &> {
	_UInt &value;
	CompressedUInt(_UInt &_v) : value(_v) {}
};



_vn_end


#endif
