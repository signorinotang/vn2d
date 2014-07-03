//
//  vnRunLoop.cpp
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnRunLoop.h"
#include <chrono>

_vn_begin

void RunLoop::run() {
    m_lock.lock();
    m_state = kRunning;
    m_lock.unlock();
    for (;;) {
        while (!m_processing.empty()) {
            Event *e = m_processing.front();
            if (e == &m_quitEvent) {
                m_processing.pop_front();
                return ;
            }
            e->process();
            m_processing.pop_front();
        }
        m_lock.lock();
        if (m_incoming.empty()) {
            m_waiting = true;
            m_cond.wait(m_lock);
        }
        m_processing.swap(m_incoming);
        m_lock.unlock();
    }
}

void RunLoop::runWithIdle(f32 interval, const std::function<void (f32)> &idle) {
    m_lock.lock();
    m_state = kRunning;
    m_lock.unlock();
    auto period = std::chrono::microseconds((s64)(interval * 1000000));
    auto last = std::chrono::steady_clock::now();
    decltype(last) next = last + period;
    for (;;) {
        while (!m_processing.empty()) {
            Event *e = m_processing.front();
            if (e == &m_quitEvent) {
                m_lock.lock();
                m_state = kQuited;
                m_processing.clear();
                m_incoming.clear();
                m_lock.unlock();
                return ;
            }
            e->process();
            m_processing.pop_front();
        }
        m_lock.lock();
        while (m_incoming.empty()) {
            m_waiting = true;
            if (m_cond.wait_until(m_lock, next) == std::cv_status::timeout) {
                m_waiting = false;
                m_lock.unlock();
                // idle.
                next = std::chrono::steady_clock::now();
                f32 dt = std::chrono::duration_cast< std::chrono::duration<f32> >(next - last).count();
                last = next;
                next = last + period;
                idle(dt);
                
                m_lock.lock();
            }
        }
        
        m_processing.swap(m_incoming);
        m_lock.unlock();
    }

}

void RunLoop::quit() {
    std::lock_guard<decltype(m_lock)> lck(m_lock);
    if (m_state != kRunning) {
        return ;
    }
    m_state = kQuiting;
    m_incoming.push_back(&m_quitEvent);
    if (m_waiting) {
        m_waiting = false;
        m_cond.notify_one();
    }
}

void RunLoop::postEvent(const EventPtr &e) {
    std::lock_guard<decltype(m_lock)> lck(m_lock);
    m_incoming.push_back(e.ptr());
    if (m_waiting) {
        m_waiting = false;
        m_cond.notify_one();
    }
}

void RunLoop::postEvent(EventPtr &&e) {
    std::lock_guard<decltype(m_lock)> lck(m_lock);
    m_incoming.push_back(e.move(), false);
    if (m_waiting) {
        m_waiting = false;
        m_cond.notify_one();
    }
}

void RunLoop::postEvents(List<vn::RunLoop::Event> &&es) {
    std::lock_guard<decltype(m_lock)> lck(m_lock);
    m_incoming.merge(es);
    if (m_waiting) {
        m_waiting = false;
        m_cond.notify_one();
    }
}


_vn_end
