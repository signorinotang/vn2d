//
//  vnSoundEngine.cpp
//  vn2d
//
//  Created by Wayne on 9/6/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSoundEngine.h"
#include "vnLog.h"
#include "vnTreeDataParser.h"
#include "vnFileManager.h"
#include <thread>

#if VN_PLATFORM == VN_PLATFORM_WIN
#	include <al.h>
#	include <alc.h>
#else
#	include <OpenAL/al.h>
#	include <OpenAL/alc.h>
#endif

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"


static std::thread *s_seThread = 0, *s_muThread = 0;
static ALCdevice *s_device = 0;
static ALCcontext *s_context = 0;

_vn_begin

static RunLoop s_music_runloop;

namespace  {
    template <typename _Func>
    struct Action : RunLoop::Event {
        _Func func;
        Action(const _Func &_func) : func(_func) {}
        Action(_Func &&_func) : func(std::forward(_func)) {}
        
        virtual void process() {
            func();
        }
    };

    template <typename _Func>
    Action<_Func> * make_action(const _Func &func) {
        return vnnew Action<_Func>(func);
    }
    
    template <typename _Func>
    Action<_Func> * make_action(_Func &&func) {
        return vnnew Action<_Func>(func);
    }
    
    size_t ds_read(void *ptr, size_t size, size_t nmemb, void *datasource) {
        return static_cast<DataStream *>(datasource)->read(ptr, size * nmemb);
    }
    
    int ds_seek(void *datasource, ogg_int64_t offset, int whence) {
        if (static_cast<DataStream *>(datasource)->seek(offset, (DataStream::SeekDir)whence)) {
            return 0;
        }
        return -1;
    }
    
    long ds_tell(void *datasource) {
        return (long)static_cast<DataStream *>(datasource)->tell();
    }
}

////////////
// sound buffer
struct SoundBuffer : public AllocatedObject {
    enum { kInit = 0, kLoading, kLoaded, kBad } state = kInit;
    u32 sid = 0;
    ALuint buffer = 0;
    FilePath file;
    
    ~SoundBuffer() {
        if (buffer) {
            alDeleteBuffers(1, &buffer);
        }
    }
    
    void load() {
        state = kLoading;
        const c8 *ext = file.fileName.c_str() + file.fileName.length() - 3;
        if (!strcmp(ext, "wav")) {
            _load_wav();
        } else if (!strcmp(ext, "ogg")) {
            _load_ogg();
        } else {
            state = kBad;
        }
        
    }
    
    void _load_wav() {
#pragma pack(push, 1)
        struct WAVHead {
            u32 chunkId;
            u32 chunkSize;
            u32 format;
            u32 subChunk1Id;
            u32 subChunk1Size;
            u16 audioFormat;
            u16 numChannels;
            u32 sampleRate;
            u32 byteRate;
            u16 blockAlign;
            u16 bitsPerSample;
            u32 subChunk2Id;
            u32 subChunk2Size;
        };
#pragma pack(pop)
        FileStream *fs = FileManager::instance().open(file);
        if (!fs) {
            state = kBad;
            return ;
        }
        
        bool ret = true;
        u8 *data = 0;
        try {
            WAVHead head;
            *fs >> DataStream::buf(&head, sizeof(head));
            if (head.chunkId != 0x46464952 ||
                head.format != 0x45564157 ||
                head.audioFormat != 1 ||
                head.subChunk2Id != 0x61746164) {
                ret = false;
            } else {
                data = vnmalloc(u8, head.subChunk2Size);
                *fs >> DataStream::buf(data, head.subChunk2Size);
                ALenum format;
                if (head.numChannels == 2) {
                    format = (head.bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);
                } else {
                    format = (head.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16);
                }
                alGenBuffers(1, &buffer);
                alBufferData(buffer, format, data, head.subChunk2Size, head.sampleRate);
            }
        } catch (const DataStream::Exception &) {
            ret = false;
        }
        if (data) {
            vnfree(data);
        }
        fs->close();
        if (ret) {
            state = kLoaded;
        } else {
            state = kBad;
        }
    }
    
    void _load_ogg() {
        FileStream *fs = FileManager::instance().open(file);
        if (!fs) {
            state = kBad;
            return ;
        }
        bool ret = true;
        u8 *data = 0;
        try {
            ov_callbacks cb = { &ds_read, &ds_seek, NULL, &ds_tell };
            OggVorbis_File vf;
            if (ov_open_callbacks(fs, &vf, NULL, 0, cb)) {
                ret = false;
            } else {
                vorbis_info *vi = ov_info(&vf, -1);
                ALenum format = (vi->channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16);
                ALsizei freq = (ALsizei)vi->rate;
                size_t data_size = (size_t)ov_pcm_total(&vf, -1) * 2 * vi->channels;
                data = vnmalloc(u8, data_size);
                u8 *ptr = data;
                long rest_size = data_size;
                int current_section;
                while (rest_size) {
                    long bytes = ov_read(&vf, (char *)ptr, (int)rest_size, 0, 2, 1, &current_section);
                    if (bytes < 0) {
                        ret = false;
                        break;
                    } else if (bytes == 0) {
                        data_size -= rest_size;
                        break;
                    }
                    rest_size -= bytes;
                    ptr += bytes;
                }
                ov_clear(&vf);
                if (ret) {
                    alGenBuffers(1, &buffer);
                    alBufferData(buffer, format, data, (ALsizei)data_size, freq);
                }
            }
            
        } catch (DataStream::Exception &) {
            ret = false;
        }
        if (data) {
            vnfree(data);
        }
        fs->close();
        if (ret) {
            state = kLoaded;
        } else {
            state = kBad;
        }
    }
    
};

///////////

SoundEngine::SoundEngine()
: m_music_mute(false)
, m_sounds_mute(false) {
    s_seThread = new std::thread([this]() {
        this->_thread();
    });
}

SoundEngine::~SoundEngine() {
    m_loop.quit();
    s_seThread->join();
    delete s_seThread;
    s_seThread = 0;
    for (auto &i : m_buffers) {
        delete i.second;
    }
}

void SoundEngine::_thread() {
    s_device = alcOpenDevice(0);
    if (!s_device) {
        return ;
    }
    s_context = alcCreateContext(s_device, 0);
    if (!s_context) {
        return ;
    }
    alcMakeContextCurrent(s_context);
    alcProcessContext(s_context);
    
    alGenSources(kNumSources, m_sources);
    
    m_loop.run();
    
    _stopAllSounds();
    _stopMusic();
    
    alDeleteSources(kNumSources, m_sources);
    
#if VN_PLATFORM == VN_PLATFORM_WIN
		return;
#endif

    if (s_context) {
        alcMakeContextCurrent(0);
        alcDestroyContext(s_context);
        s_context = 0;
    }
    if (s_device) {
        alcCloseDevice(s_device);
        s_device = 0;
    }
}

void SoundEngine::suspend() {
    m_loop.postEvent(make_action([this]() {
        alcSuspendContext(s_context);
    }));
}

void SoundEngine::resume() {
    m_loop.postEvent(make_action([this]() {
        alcProcessContext(s_context);
    }));
}

void SoundEngine::loadSoundTable(const vn::FilePath &file) {
    TreeDataNode *root = TreeDataParser::instance().parse(file);
    if (!root) {
        VN_LOG_ERROR("SoundEngine: load sound table \"" << file.getFullPath() << "\" failed!");
        return ;
    }
    const TreeDataArray *tda = root->queryAttributeArray("sounds");
    if (tda) {
        for (ssize_t i = 0, c = tda->size() - 1; i < c; i += 2) {
            const TreeDataInteger *tdn = dynamic_cast<const TreeDataInteger *>(tda->at(i));
            u32 sid = 0;
            if (tdn) {
                sid = tdn->getInteger();
            }
            const TreeDataString * tdstr = dynamic_cast<const TreeDataString *>(tda->at(i + 1));
            if (sid && tdstr) {
                SoundBuffer *&p = m_buffers.insert(std::make_pair(sid, (SoundBuffer *)0)).first->second;
                if (!p) {
                    p = vnnew SoundBuffer();
                    p->sid = sid;
                }
                p->file = tdstr->value().c_str();
            }
        }
    }
    
    tda = root->queryAttributeArray("music");
    if (tda) {
        for (ssize_t i = 0, c = tda->size() - 1; i < c; i += 2) {
            const TreeDataInteger *tdn = dynamic_cast<const TreeDataInteger *>(tda->at(i));
            u32 sid = 0;
            if (tdn) {
                sid = tdn->getInteger();
            }
            const TreeDataString * tdstr = dynamic_cast<const TreeDataString *>(tda->at(i + 1));
            if (sid && tdstr) {
                m_files[sid] = tdstr->value().c_str();
            }
        }
    }

    
    root->drop();
}

void SoundEngine::playSound(u32 sid) {
    m_play.insert(sid);
}

void SoundEngine::playLoopSound(u32 sid) {
    auto it = m_stopLoop.find(sid);
    if (it != m_stopLoop.end()) {
        if (!--it->second) {
            m_stopLoop.erase(it);
        }
        return ;
    }
    ++m_playLoop.insert(std::make_pair(sid, 0)).first->second;
}

void SoundEngine::stopLoopSound(u32 sid) {
    auto it = m_playLoop.find(sid);
    if (it != m_playLoop.end()) {
        if (!--it->second) {
            m_playLoop.erase(it);
        }
        return ;
    }
    ++m_playLoop.insert(std::make_pair(sid, 0)).first->second;
}

void SoundEngine::stopAllSounds() {
    m_loop.postEvent(make_action([this](){
        _stopAllSounds();
    }));
}

void SoundEngine::playMusic(u32 mid) {
    m_loop.postEvent(make_action([this, mid](){
        _playMusic(mid);
    }));
}

void SoundEngine::stopMusic() {
    m_loop.postEvent(make_action([this](){
        _stopMusic();
    }));
}

void SoundEngine::setMusicMute(bool mute) {
    m_music_mute = mute;
    m_loop.postEvent(make_action([this, mute]() {
        if (m_music_id) {
            s_music_runloop.postEvent(make_action([this, mute](){
                alSourcef(m_sources[kNumSources], AL_GAIN, (mute ? 0.f : 1.f));
            }));
        }
    }));
}

void SoundEngine::setSoundsMute(bool mute) {
    m_sounds_mute = mute;
    m_loop.postEvent(make_action([this, mute](){
        f32 gain = (mute ? 0.f : 1.f);
        for (int i = 0; i < kNumSources; ++i) {
            alSourcef(m_sources[i], AL_GAIN, gain);
        }
    }));
}

bool SoundEngine::isMusicMute() {
    return m_music_mute;
}

bool SoundEngine::isSoundsMute() {
    return m_sounds_mute;
}

void SoundEngine::update() {
    if (!m_play.empty()) {
        struct PlayEvent : RunLoop::Event {
            SoundEngine *se;
            std::set<u32> ids;
            PlayEvent(SoundEngine *_se, std::set<u32> &&_ids) : se(_se), ids(std::move(_ids)) {}
            virtual void process() {
                se->_play(ids);
            }
        };
        m_loop.postEvent(vnnew PlayEvent(this, std::move(m_play)));
    }
    if (!m_playLoop.empty()) {
        struct PlayLoopEvent : RunLoop::Event {
            SoundEngine *se;
            std::map<u32, s32> ids;
            PlayLoopEvent(SoundEngine *_se, std::map<u32, s32> &&_ids) : se(_se), ids(std::move(_ids)) {}
            virtual void process() {
                se->_playLoop(ids);
            }
        };
        m_loop.postEvent(vnnew PlayLoopEvent(this, std::move(m_playLoop)));
    }
    if (!m_stopLoop.empty()) {
        struct StopLoopEvent : RunLoop::Event {
            SoundEngine *se;
            std::map<u32, s32> ids;
            StopLoopEvent(SoundEngine *_se, std::map<u32, s32> &&_ids) : se(_se), ids(std::move(_ids)) {}
            virtual void process() {
                se->_playLoop(ids);
            }
        };
        m_loop.postEvent(vnnew StopLoopEvent(this, std::move(m_stopLoop)));
    }
}

void SoundEngine::_play(const std::set<u32> &ids) {
    for (auto i : ids) {
        auto it = m_buffers.find(i);
        if (it == m_buffers.end()) {
            continue;
        }
        SoundBuffer *buf = it->second;
        if (buf->state == SoundBuffer::kInit) {
            buf->load();
        }
        if (buf->state == SoundBuffer::kLoaded) {
            bool ok = false;
            for (int i = 0; i < kNumSources; ++i) {
                ALenum state;
                alGetSourcei(m_sources[i], AL_SOURCE_STATE, &state);
                if (state == AL_INITIAL || state == AL_STOPPED) {
                    alSourcei(m_sources[i], AL_BUFFER, buf->buffer);
                    alSourcei(m_sources[i], AL_LOOPING, AL_FALSE);
                    alSourcePlay(m_sources[i]);
                    ok = true;
                    break;
                }
            }
            if (!ok) {
                break;
            }
        }
    }
}

void SoundEngine::_playLoop(const std::map<u32, s32> &ids) {
    for (auto &i : ids) {
        auto j = m_looping.find(i.first);
        if (j != m_looping.end()) {
            j->second.count += i.second;
            continue;
        }
        auto it = m_buffers.find(i.first);
        if (it == m_buffers.end()) {
            continue;
        }
        SoundBuffer *buf = it->second;
        if (buf->state == SoundBuffer::kInit) {
            buf->load();
        }
        if (buf->state == SoundBuffer::kLoaded) {
            bool ok = false;
            for (int idx = 0; idx < kNumSources; ++idx) {
                ALenum state;
                alGetSourcei(m_sources[idx], AL_SOURCE_STATE, &state);
                if (state == AL_INITIAL || state == AL_STOPPED) {
                    alSourcei(m_sources[idx], AL_BUFFER, buf->buffer);
                    alSourcei(m_sources[idx], AL_LOOPING, AL_TRUE);
                    alSourcePlay(m_sources[idx]);
                    m_looping.insert(std::make_pair(i.first, LoopInfo(idx, i.second)));
                    ok = true;
                    break;
                }
            }
            if (!ok) {
                break;
            }
        }
    }
}

void SoundEngine::_stopLoop(const std::map<u32, s32> &ids) {
    for (auto &i : ids) {
        auto j = m_looping.find(i.first);
        if (j == m_looping.end()) {
            continue;
        }
        if ((j->second.count -= i.second) <= 0) {
            alSourceStop(m_sources[j->second.index]);
            m_looping.erase(j);
        }
    }
}

void SoundEngine::_stopAllSounds() {
    for (size_t i = 0; i < kNumSources; ++i) {
        alSourceStop(m_sources[i]);
    }
    m_looping.clear();
}



void SoundEngine::_playMusic(u32 mid) {
    auto it = m_files.find(mid);
    if (it == m_files.end()) {
        return ;
    }
    if (s_muThread) {
        if (m_music_id == mid) {
            return ;
        }
        s_music_runloop.quit();
        s_muThread->join();
        delete s_muThread;
        s_muThread = 0;
    }
    m_music_id = mid;
    s_muThread = new std::thread([this](const FilePath &file){
        _music_thread(file);
    }, it->second);
}

void SoundEngine::_stopMusic() {
    if (s_muThread) {
        s_music_runloop.quit();
        s_muThread->join();
        delete s_muThread;
        s_muThread = 0;
    }
}

void SoundEngine::_music_thread(const FilePath &file) {
    FileStream *fs = FileManager::instance().open(file);
    if (!fs) {
        m_music_id = 0;
        return ;
    }
    
    ov_callbacks cb = { &ds_read, &ds_seek, NULL, &ds_tell };
    OggVorbis_File vf;
    if (ov_open_callbacks(fs, &vf, NULL, 0, cb)) {
        fs->close();
        m_music_id = 0;
        return ;
    }
    vorbis_info *vi = ov_info(&vf, -1);
    ALenum format = (vi->channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16);
    ALsizei freq = (ALsizei)vi->rate;
    
    size_t sample_size = 2 * vi->channels;
    size_t buffer_size = freq / 4 * sample_size;
    
    ALuint buffers[4];
    alGenBuffers(4, buffers);
    c8 *buf = vnmalloc(c8, buffer_size);
    int count = 0;
    int current_section;
    bool finish = false;
    for (int i = 0; i < 4 && !finish; ++i) {
        c8 *ptr = buf;
        size_t data_size = 0;
        size_t rest = buffer_size;
        while (rest) {
            long bytes = ov_read(&vf, ptr, (int)rest, 0, 2, 1, &current_section);
            if (bytes < 0) {
                finish = true;
                break;
            } else if (bytes == 0) {
                ov_raw_seek(&vf, 0);
            } else {
                data_size += bytes;
                ptr += bytes;
                rest -= bytes;
            }
        }
        if (data_size) {
            alBufferData(buffers[count++], format, buf, (ALsizei)data_size, freq);
        }
    }
    
    ALuint &source = m_sources[kNumSources];
    alGenSources(1, &source);
    if (m_music_mute) {
        alSourcef(source, AL_GAIN, 0.f);
    }
    
    alSourceQueueBuffers(source, count, buffers);
    
    alSourcePlay(source);
    
    s_music_runloop.runWithIdle(0.25f, [&](f32 deltaTime) {
        ALint processed = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
        if (processed) {
            ALuint processed_buffers[4];
            alSourceUnqueueBuffers(source, processed, processed_buffers);
            count = 0;
            while (count < processed && !finish) {
                c8 *ptr = buf;
                size_t data_size = 0;
                size_t rest = buffer_size;
                while (rest) {
                    long bytes = ov_read(&vf, ptr, (int)rest, 0, 2, 1, &current_section);
                    if (bytes < 0) {
                        finish = true;
                        s_music_runloop.quit();
                        break;
                    } else if (bytes == 0) {
                        ov_raw_seek(&vf, 0);
                    } else {
                        data_size += bytes;
                        ptr += bytes;
                        rest -= bytes;
                    }
                }
                if (data_size) {
                    alBufferData(processed_buffers[count++], format, buf, (ALsizei)data_size, freq);
                }
            }
            alSourceQueueBuffers(source, count, processed_buffers);
        }
    });
    
    alSourceStop(source);
    alDeleteSources(1, &source);
    source = 0;
    
    vnfree(buf);
    ov_clear(&vf);
    fs->close();
    m_music_id = 0;
    alDeleteBuffers(4, buffers);

}

_vn_end
