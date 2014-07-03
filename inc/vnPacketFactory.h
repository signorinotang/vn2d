//
//  vnPacketFactory.h
//  vnlib
//
//  Created by Wayne on 14-5-24.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnPacketFactory_h
#define vnlib_vnPacketFactory_h

#include "vnPacket.h"
#include "vnSingleton.h"

#include <unordered_map>

_vn_begin

template <class _Handler>
class PacketFactory : public Singleton< PacketFactory<_Handler> > {
public:
	
	template <class _Packet>
	void registerRequest() {
		class PacketEx : _Packet {
		public:
			static Packet * create() { return vnnew PacketEx(); }
			virtual long process(void *param) {
				return static_cast<_Handler *>(param)->onRequest((_Packet *)this);
			}
		};
		m_requests[_Packet::ID] = PacketEx::create;
	}
	
	template <class _Packet>
	void registerResponse() {
		class PacketEx : _Packet {
		public:
			static Packet * create() { return vnnew PacketEx(); }
			virtual long process(void *param) {
				return static_cast<_Handler *>(param)->onResponse((_Packet *)this);
			}
		};
		m_responses[_Packet::ID] = PacketEx::create;
	}
	
	PacketPtr createRequest(u32 pid) {
		auto it = m_requests.find(pid);
		if (it == m_requests.end()) {
			return PacketPtr();
		}
		return it->second();
	}
	
	PacketPtr createResponse(u32 pid) {
		auto it = m_responses.find(pid);
		if (it == m_responses.end()) {
			return PacketPtr();
		}
		return it->second();
	}
	
private:
	std::unordered_map<u32, Packet *(*)()> m_requests, m_responses;
};

_vn_end

#define VN_DECL_PACKET_HANDLER() \
	public:\
		template <class _Packet>\
		long onRequest(_Packet *);\
		template <class _Packet>\
		long onResponse(_Packet *);

#define VN_IMPL_PACKET_REQUEST(handler, packet) \
	static struct _auto_reg_##handler##_req_##packet {\
		_auto_reg_##handler##_req_##packet() {\
			_vn_ns::PacketFactory<handler>::instance().registerRequest<packet>();\
		}\
	} _auto_reg_##handler##_req_##packet##_ins;\
	template<> long handler::onRequest<packet>(packet *pkt)

#define VN_IMPL_PACKET_RESPONSE(handler, packet) \
	static struct _auto_reg_##handler##_res_##packet {\
		_auto_reg_##handler##_res_##packet() {\
			_vn_ns::PacketFactory<handler>::instance().registerResponse<packet>();\
		}\
	} _auto_reg_##handler##_res_##packet##_ins;\
	template<> long handler::onResponse<packet>(packet *pkt)

#endif
