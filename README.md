# Styler

My old unfinished project.
It was supposed to play auto-accompaniment styles similarly to a musical keyboard, but by using prerecorded audio instead of sound synthesis.
Project was abandoned because i couldn't figure out how to time-scale audio in real time (to allow the user to set the tempo of the accompaniment) without huge latency.

Since i didn't know what to do with all this code i wrote and didn't want to just delete it, i put it here :). 
Who knows, maybe it will inspire someone to make a similar program that works?

Libraries used:
- GTK for UI
- PortAudio for audio UI
- nlohmann::json for parsing JSON
- libsndfile for reading audio files
