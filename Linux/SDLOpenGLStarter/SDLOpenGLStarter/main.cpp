#define SDL_MAIN_HANDLED
#include "GameEngine.h"
// ADD THIS CODE FOR TASK 2
#include <sstream>

using namespace GE;

int main() {
	// Create a game engine object
	GameEngine ge;

	// Initialise it
	if (!ge.init()) {
		display_info_message("Couldn't start SDL.  Check console output for more details");

		return -1;
	}

	// ADD THIS CODE FOR TASK 2: These variables store the time at two points in the program
	Uint32 last_time = SDL_GetTicks(), current_time = 0;

	// ADD THIS CODE FOR TASK 2: Simple frame counter
	int frame_count = 0;

	// Main loop
	while (ge.keep_running()) {
		// Update game state
		ge.update();

		// Draw
		ge.draw();

		// ADD THIS CODE FOR TASK 2: done a frame so update counter
		frame_count++;

		// ADD THIS CODE FOR TASK 2: get the time at this point
		current_time = SDL_GetTicks();

		// ADD THIS CODE FOR TASK 2: is the difference between the two times 1000
		if (current_time - last_time > 1000) {
			// ADD THIS CODE FOR TASK 2: create a variable to store a string
			std::ostringstream msg;

			// ADD THIS CODE FOR TASK 2: create a message based on the value in frame_count
			msg << "FPS = " << frame_count;

			// ADD THIS CODE FOR TASK 2: Display the message in the window title
			ge.setwindowtitle(msg.str().c_str());

			// ADD THIS CODE FOR TASK 2: Reset frame counter
			frame_count = 0;

			// ADD THIS CODE FOR TASK 2: Update time variable for next count
			last_time = current_time;
		}
	}

	// Destroy game engine
	ge.shutdown();

	return 0;
}