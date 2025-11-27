#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include "PointerWrapper.h"
#include <iostream>
#include <memory>
#include <filesystem>



DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}


DJLibraryService::~DJLibraryService() {
    for (AudioTrack* t : library) {
        delete t;
    }
    library.clear();
}


/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for (int i = 0; i < library_tracks.size(); i++){
        std:: string type = library_tracks[i].type;
        AudioTrack* new_track = nullptr;
        if (type == "MP3"){
            new_track = new MP3Track(library_tracks[i].title,library_tracks[i].artists,
            library_tracks[i].duration_seconds,library_tracks[i].bpm,
            library_tracks[i].extra_param1,library_tracks[i].extra_param2);
            std::cout << "MP3Track created: " << library_tracks[i].extra_param1 << " kbps \n";
        }  
        else {
            new_track = new WAVTrack(library_tracks[i].title,library_tracks[i].artists,
            library_tracks[i].duration_seconds,library_tracks[i].bpm,
            library_tracks[i].extra_param1,library_tracks[i].extra_param2);
            std::cout << "WAVTrack created: " << library_tracks[i].extra_param1 << "Hz/" << library_tracks[i].extra_param2 << "bit \n";
        }
        this->library.push_back(new_track);  
    }
    std::cout << "[INFO] Track library built: " << this->library.size() << " tracks loaded \n";
}
    


/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name  <<"\n";
    playlist = Playlist(playlist_name);
    for (int i = 0; i < track_indices.size() ; i++) {
        int index = track_indices[i];
        if (index <= library.size() && index > 0) {
           AudioTrack* to_insert = library[index -1] ; 
           PointerWrapper<AudioTrack> new_track_wrapper = to_insert->clone();
           AudioTrack* new_track = new_track_wrapper.release();
           if (!new_track) {
            std::cout << " [ERROR] Cloned track return null. \n";
           }
           else {
                new_track->load();
                new_track->analyze_beatgrid();
                playlist.add_track(new_track);
                std::cout << "Added " << new_track->get_title() << " to playlist " << playlist_name << "\n";
           }
        }
        else {
            std::cout << "[WARNING] Invalid track index: " << std::to_string(index) << "\n";
        }
    }
    std::cout << "[INFO] Playlist Loaded: " << playlist_name << "( " << playlist.get_track_count() << " tracks) \n";
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    std::vector<std::string> titles;
    for (int i = 0; i < tracks.size(); i++){
        titles.push_back(tracks[i]->get_title());
    }  
    return titles;
}