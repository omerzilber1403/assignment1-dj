#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }   
    PointerWrapper<AudioTrack> cloneWrapper = track.clone();
    if (!cloneWrapper){
        "DJControllerService: clone() failed for track '" + track.get_title() + "'";
        return -1;
    }
    AudioTrack* clonedRaw = cloneWrapper.release();
    clonedRaw->load();                 
    clonedRaw->analyze_beatgrid(); 
    PointerWrapper<AudioTrack> preparedClone(clonedRaw);
    bool evicted = cache.put(std::move(preparedClone));

    if (evicted) {
        return -1;
    } else {
        return 0;
    }    
    return 0; 
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack* track = cache.get(track_title);
    if (!track) {
        std::cout << "[ERROR] Track: " + track_title + "  not found in cache\n";
    }
    return track;
}
