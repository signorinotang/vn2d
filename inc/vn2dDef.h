//
//  vn2dDef.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vn2dDef_h
#define vn2d_vn2dDef_h

#include <cstddef>
#include "vnPlatform.h"

#define _vn_begin 	namespace vn {
#define _vn_end		}
#define _vn_ns		::vn

#define VN_2D

#ifdef VN_2D_DYNLIB
#	if VN_COMPILER == VN_COMPILER_MSVC
#		ifdef VN_2D_EXPORT
#			define _vn_base_api __declspec(dllexport)
#           define _vn_gfx_api   __declspec(dllexport)
#           define _vn_2d_api   __declspec(dllexport)
#		else
#			define _vn_base_api __declspec(dllimport)
#			define _vn_gfx_api __declspec(dllimport)
#           define _vn_2d_api   __declspec(dllimport)
#		endif
#	elif VN_COMPILER == VN_COMPILER_LLVM || VN_COMPILER == VN_COMPILER_GUNC
#		ifdef VN_BASE_EXPORT
#			define _vn_base_api __attribute__((visibility("default")))
#           define _vn_gfx_api __attribute__((visibility("default")))
#           define _vn_2d_api   __attribute__((visibility("default")))
#		else
#			define _vn_base_api
#			define _vn_gfx_api
#           define _vn_2d_api
#		endif
#	else
#		define _vn_base_api
#		define _vn_gfx_api
#       define _vn_2d_api
#	endif
#else
#	define _vn_base_api
#	define _vn_gfx_api
#   define _vn_2d_api
#endif

#ifdef VN_DEBUG
#   define VN_MEMORY_TRACKER
#endif

#if VN_COMPILER == VN_COMPILER_MSVC
#	pragma warning(disable:4251)
#endif

#endif
