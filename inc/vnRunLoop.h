//
//  vnRunLoop.h
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnRunLoop_h
#define vnlib_vnRunLoop_h

#include "vnList.h"
#include "vnFastMutex.h"

#include <condition_variable>


_vn_begin

class RunLoop : public AllocatedObject {
public:
    class Event : public List<Event>::Node {
    public:
        virtual void process() {}
    };
    typedef RefCountedPtr<Event> EventPtr;
    
    enum State {
        kQuited = 0,
        kRunning,
        kQuiting
    };
    
    State state() const { return m_state; }
    
    void run();
    void runWithIdle(f32 interval, const std::function<void (f32)> &idle);
    void quit();
    
    void postEvent(const EventPtr &e);
    void postEvent(EventPtr &&e);
    void postEvents(List<Event> &&es);
    
    
protected:
    std::condition_variable_any m_cond;
    FastMutex m_lock;
    List<Event> m_incoming;
    List<Event> m_processing;
    Event m_quitEvent;
    bool m_waiting = false;
    State m_state = kQuited;
};

_vn_end


#endif
