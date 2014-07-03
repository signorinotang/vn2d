//
//  vnPacket.h
//  vnlib
//
//  Created by Wayne on 14-5-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnPacket_h
#define vnlib_vnPacket_h

#include "vnRefCounted.h"
#include "vnDataStream.h"

_vn_begin

class _vn_base_api Packet : public RefCounted {
public:
	virtual u32 pid() const = 0;
	virtual void save(DataStream &stream) const {}
	virtual void load(DataStream &stream) {}
	virtual long process(void *param) { return 0; }
};

typedef RefCountedPtr<Packet> PacketPtr;

_vn_end

#endif
