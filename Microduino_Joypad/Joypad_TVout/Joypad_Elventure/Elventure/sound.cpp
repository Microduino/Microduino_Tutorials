#include <TVout.h>
#include "sound_data.h"
#include "sound.h"

#define SONG_PLAYING 0
#define SONG_ENDED 1
#define SONG_PLAYING_ONCE 2
#define SONG_PLAYING_SFX 3

#define MUSIC_STOPPED 4

extern TVout TV;

int song_note_len;       //current song's not length
int current_note;        //current note
char current_song;       //current song
int song_end_pos;        //indicates the position of the last note of the song
char music_state;        //indicates the current music state

//these are used when a sound effect is being played to store the old state
int old_song_note_len;
int old_current_note;
int old_song_end_pos;
char old_music_state;
char old_current_song;

//handles the music/sfx playing for the sketch
void update_sound()
{
  switch (music_state)
  {
  case SONG_PLAYING:
  case SONG_PLAYING_ONCE:
  case SONG_PLAYING_SFX:
    static long next_note_start_time = 0;
    if (TV.millis() >= next_note_start_time)
    {
      if (current_note == song_end_pos)
      {
         switch (music_state)
         {
	    //it's the end of the song, so we can contine
	    case SONG_PLAYING:
	         music_state = SONG_ENDED;
                  break;
			  
	    case SONG_PLAYING_ONCE:
                 music_state = MUSIC_STOPPED;
                 break;	

           case SONG_PLAYING_SFX:
		//restore the old state of the music and continue
		song_note_len = old_song_note_len;
		song_end_pos = old_song_end_pos;
		current_note = old_current_note;
		music_state = old_music_state; 
                current_song = old_current_song;
                break;		   
	 }
      }
      
     //determine the current note duration
      int note_duration = ( song_note_len / pgm_read_byte_near(duration + current_note));
  	  
      next_note_start_time = TV.millis()+note_duration;
	  
	  //play the note
      TV.tone(pgm_read_word_near(melody + current_note), note_duration); 
  	
      current_note++;  
    }
    break;
	
   case SONG_ENDED:
     //start the same song over automatically
     play_song(current_song);
	 break;
   
   case MUSIC_STOPPED:
     //we are idle, until a new play action is initiated
     break;
  }
}

//starts the playing of the selected song
void play_song(char song)
{
  //determine the note length based on the tempo of the song
  song_note_len = ( 60000 / pgm_read_word_near(tempo + song)) * 4;
  
  //determine the end position of the song
  song_end_pos = pgm_read_word_near(song_start + (song+1)) - 1;
  
  //set the note to the first note of the song
  current_note = pgm_read_word_near(song_start + song);
  
  //set the state to 'playing'
  music_state = SONG_PLAYING;
  
  //set the current song value to the passed parameter
  current_song = song;
}

//plays a song only once
void play_song_once(char song)
{
  play_song(song);
  //override the state to a single time through
  music_state = SONG_PLAYING_ONCE;
}

//plays a sound effect
void play_sfx(char song)
{
  //saves the current state of the song to be restored
  //when the sound effect is finished playing
  old_song_note_len = song_note_len;
  old_song_end_pos = song_end_pos;
  old_current_note = current_note;
  old_music_state = music_state;
  old_current_song = current_song;
  
  play_song(song);
  
  //override the state to a single time through
  music_state = SONG_PLAYING_SFX;
}
