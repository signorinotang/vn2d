//
//  vnSoundEngine.h
//  vn2d
//
//  Created by Wayne on 9/6/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSoundEngine_h
#define vn2d_vnSoundEngine_h

#include "vnSingleton.h"
#include "vnRunLoop.h"
#include "vnFilePath.h"
#include <map>
#include <set>

_vn_begin

struct SoundBuffer;

class _vn_2d_api SoundEngine : public Singleton<SoundEngine> {
    friend class Singleton<SoundEngine>;
public:
    enum { kNumSources = 8 };

    ~SoundEngine();
    
    void suspend();
    void resume();
    
    void loadSoundTable(const FilePath &file);
    
    void playSound(u32 sid);
    void playLoopSound(u32 sid);
    void stopLoopSound(u32 sid);
    void stopAllSounds();
    
    void playMusic(u32 mid);
    void stopMusic();
    
    void setMusicMute(bool mute);
    void setSoundsMute(bool mute);
    
    bool isMusicMute();
    bool isSoundsMute();
    
    void update();
    
private:
    SoundEngine();
    RunLoop m_loop;
    
    void _thread();
    
    u32 m_sources[kNumSources + 1];
    std::map<u32, SoundBuffer *> m_buffers;
    std::map<u32, FilePath> m_files;
    
    std::set<u32> m_play;
    std::map<u32, s32> m_playLoop, m_stopLoop;
    struct LoopInfo {
        u32 index;
        s32 count;
        LoopInfo(u32 idx, s32 c) : index(idx), count(c) {}
    };
    std::map<u32, LoopInfo> m_looping;
    
    void _play(const std::set<u32> &ids);
    void _playLoop(const std::map<u32, s32> &ids);
    void _stopLoop(const std::map<u32, s32> &ids);
    void _stopAllSounds();
    void _playMusic(u32 mid);
    void _stopMusic();
    
    void _music_thread(const FilePath &file);
    
    u32 m_music_id;
    
    bool m_music_mute;
    bool m_sounds_mute;
};

_vn_end


#endif
