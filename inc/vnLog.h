//
//  vnLog.h
//  vnbase
//
//  Created by Wayne on 13-12-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnLog_h
#define vnbase_vnLog_h

#include "vnSingleton.h"
#include "vnRefCounted.h"
#include "vnFastMutex.h"
#include <vector>
#include <sstream>

#define VN_LOG_DEBUG(x) \
{\
_vn_ns::Log &_log = _vn_ns::Log::instance();\
if (_log.checkLevel(_vn_ns::Log::kDebug)) {\
_vn_ns::Log::stream(_log, _vn_ns::Log::kDebug) << x;\
}\
}

#define VN_LOG_INFO(x) \
{\
_vn_ns::Log &_log = _vn_ns::Log::instance();\
if (_log.checkLevel(_vn_ns::Log::kInformation)) {\
_vn_ns::Log::stream(_log, _vn_ns::Log::kInformation) << x;\
}\
}

#define VN_LOG_WARN(x) \
{\
_vn_ns::Log &_log = _vn_ns::Log::instance();\
if (_log.checkLevel(_vn_ns::Log::kWarning)) {\
_vn_ns::Log::stream(_log, _vn_ns::Log::kWarning) << x; \
}\
}

#define VN_LOG_ERROR(x) \
{\
_vn_ns::Log &_log = _vn_ns::Log::instance();\
if (_log.checkLevel(_vn_ns::Log::kError)) {\
_vn_ns::Log::stream(_log, _vn_ns::Log::kError) << x; \
}\
}

#define VN_LOG_FATAL(x) \
{\
_vn_ns::Log &_log = _vn_ns::Log::instance();\
if (_log.checkLevel(_vn_ns::Log::kFatalError)) {\
_vn_ns::Log::stream(_log, _vn_ns::Log::kFatalError) << x; \
}\
}

_vn_begin

class LogPrinter;

class _vn_base_api Log : public Singleton<Log>  {
public:
    enum Level {
        kDebug = 0,
        kInformation,
        kWarning,
        kError,
        kFatalError
    };
    class _vn_base_api stream {
        friend class Log;
    public:
        stream(Level);
        stream(Log &, Level);
        ~stream();
        
        stream & operator <<(bool v);
        stream & operator <<(s8 v);
        stream & operator <<(u8 v);
        stream & operator <<(s16 v);
        stream & operator <<(u16 v);
        stream & operator <<(s32 v);
        stream & operator <<(u32 v);
        stream & operator <<(s64 v);
        stream & operator <<(u64 v);
        stream & operator <<(f32 v);
        stream & operator <<(f64 v);
		stream & operator <<(long v);
		stream & operator <<(unsigned long v);
        stream & operator <<(const c8 *v);
        stream & operator <<(const str8 &v);
        stream & operator <<(const c16 *v);
        stream & operator <<(const str16 &v);
        stream & operator <<(const c32 *v);
        stream & operator <<(const str32 &v);
    private:
        Level m_level;
        Log &m_log;
        std::basic_ostringstream<c8> m_oss;
    };
    
    Log();
    ~Log();
    void addPrinter(LogPrinter *printer, bool grab = true);
    void removePrinter(LogPrinter *printer);
    void removeAllPrinters();
    
    bool checkLevel(Level ll);
    
    void setLevel(Level ll);
    Level getLevel() const;
private:
    void _output(Level ll, const str8 &content);
    
    friend class stream;
    
    typedef std::vector<LogPrinter *> Printers;
    Printers m_printers;
    Level m_level;
};

class _vn_base_api LogPrinter : public RefCounted {
public:
    void print(Log::Level, const str8 &);
protected:
    virtual void _print(Log::Level, const str8 &) = 0;
private:
    FastMutex m_lock;
};

class _vn_base_api LogPrinterSTDOUT : public LogPrinter {
protected:
    virtual void _print(Log::Level, const str8 &);
};

_vn_end

#endif
