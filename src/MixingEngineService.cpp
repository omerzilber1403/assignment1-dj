#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    set_auto_sync(false);
    set_bpm_tolerance(0);
    std:: cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std:: cout << "[MixingEngineService] Cleaning up decks....\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i]) {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std:: cout << "=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> track_pointer = track.clone();
    if (!track_pointer) {
        std:: cout << "[ERROR] Track: " << track.get_title() << " failed to clone\n";
        return -1;
    }
    size_t target_deck = 1 - active_deck;
    std:: cout << " [Deck Switch] Target deck: " << target_deck << "\n";
    if ( decks[target_deck] ) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    track_pointer->load();
    track_pointer->analyze_beatgrid();
    if (decks[active_deck] && auto_sync && !can_mix_tracks(track_pointer)) {
        sync_bpm(track_pointer);
    }
    decks[target_deck] = track_pointer.release();
    std:: cout << " [Load Complete] " << decks[target_deck]->get_title() << " is now loaded on deck " << std::to_string(target_deck) << "\n";
    if ( decks[active_deck] ) {
        std:: cout << " [Unload] Unloading previous deck " << std::to_string(active_deck) << " (" << decks[active_deck]->get_title() << ")\n";
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = target_deck;
    std:: cout << " [Active Deck] Switched to deck " << std::to_string(target_deck);
    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (!decks[active_deck] || !track){
        return false;
    }
    int active_bpm = decks[active_deck]->get_bpm(); 
    int new_track_bpm = track.get()->get_bpm();
    int diff = std:: abs(new_track_bpm - active_bpm);
    
    return diff <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
     if (!decks[active_deck] || !track){
        return;
    }
    int active_bpm = decks[active_deck]->get_bpm(); 
    int new_track_bpm = track.get()->get_bpm();
    int avg = (new_track_bpm + active_bpm) / 2 ;
    track.get()->set_bpm(avg);
    std:: cout << "[Sync BPM] Syncing BPM from " + std:: to_string(new_track_bpm) + " to " + std:: to_string(avg) + "\n";
}