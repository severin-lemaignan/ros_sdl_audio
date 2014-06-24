
#include "ros/ros.h"
#include "std_msgs/String.h"

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

class AudioPlayer {

	Mix_Chunk *sound = NULL;		//Pointer to our sound, in memory
	int channel;				//Channel on which our sound is played
	  
	int audio_rate = 16000;			//Frequency of audio playback
	Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
	int audio_channels = 1;			//2 channels = stereo
	int audio_buffers = 4096;		//Size of the audio buffers in memory

public:
	AudioPlayer() {
		
		//Initialize SDL_mixer with our chosen audio settings
		if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
			ROS_ERROR("Unable to initialize audio: %s\n", Mix_GetError());
			exit(1);
		}
		
	}	

	~AudioPlayer() {
		//Need to make sure that SDL_mixer and SDL have a chance to clean up
		Mix_CloseAudio();
	}

	void playCb(const std_msgs::String::ConstPtr& msg) {

		//Load our WAV file from disk
		sound = Mix_LoadWAV(msg->data.c_str());
		if(sound == NULL) {
			ROS_ERROR("Unable to load WAV file: %s\n", Mix_GetError());
		}
		
		//Play our sound file, and capture the channel on which it is played
		channel = Mix_PlayChannel(-1, sound, 0);
		if(channel == -1) {
			ROS_ERROR("Unable to play WAV file: %s\n", Mix_GetError());
		}
		
		//Wait until the sound has stopped playing
		while(Mix_Playing(channel) != 0);
		
		//Release the memory allocated to our sound
		Mix_FreeChunk(sound);

	}

};

int main(int argc, char *argv[])
{

	if (SDL_Init( SDL_INIT_AUDIO) != 0) {
		ROS_ERROR("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	ros::init(argc, argv, "audio_player");
	ros::NodeHandle n;

	AudioPlayer player;

	ros::Subscriber sub = n.subscribe("play_audio_file", 1, &AudioPlayer::playCb, &player);

	ros::spin();

	SDL_Quit();	
	
	//Return success!
	return 0;
}
