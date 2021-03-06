#include<iostream>
#include<allegro5\allegro.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<time.h>
#include<stdlib.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include<allegro5\allegro_primitives.h>
#include<stdio.h>
#include<allegro5\allegro_image.h>

using namespace std;
const int screen_W = 640;
const int screen_H = 480;
const int square_size = 32;

enum STATES { DEATH,  MAP, INTRO, MAP2, MAP3 }; //death is 0, map is 1, intro is 2,MAP@ is 3, MAP3 is 4.
int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *square = NULL;
	ALLEGRO_BITMAP *background = NULL;
	ALLEGRO_SAMPLE *sample1 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance1 = NULL;
	ALLEGRO_BITMAP *background2 = NULL;
	ALLEGRO_BITMAP *background3 = NULL;


	int state = 2;
	//these two variables hold the x and y positions of the square
	//initalize these variables to where you want your square to start
	float square_x = 50;
	float square_y = 50;


	bool isOnSolidGround = 0;

	float velocity_vx = 0;
	float velocity_vy = 0;


	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	bool key[5] = { false, false, false, false,false };


	//don't redraw until an event happens
	bool redraw = true;

	//this controls our game loop
	bool doexit = false;

	int WorldHeight = 480;
	int WorldWidth = 5000;

	int cameraX = 0;
	int cameraY = 0;

	al_init();

	//get the keyboard ready to use
	al_install_keyboard();

	al_install_audio();

	al_init_acodec_addon();

	al_init_primitives_addon();

	al_init_image_addon();

	timer = al_create_timer(.02);

	display = al_create_display(screen_W, screen_H);

	square = al_create_bitmap(square_size, square_size);



	background = al_load_bitmap("map level 1.png");

	background2 = al_load_bitmap("map level 2.png");

	background3 = al_load_bitmap("map level 3.png");

	al_reserve_samples(5);

	sample1 = al_load_sample("soviet-anthem1944.wav");

	instance1 = al_create_sample_instance(sample1);

	al_set_sample_instance_playmode(instance1, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());

	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", 32, 0);

	al_convert_mask_to_alpha(square, al_map_rgb(255, 255, 255));

	al_set_target_bitmap(square);

	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);




	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels

			if (state == INTRO) {
				if (key[4] == true)
					state = 1;
			}

			if (state == MAP) {


				if (isOnSolidGround) {
					velocity_vx *= .00; //slide 
					if (key[0]) {
						velocity_vy = -10;
						isOnSolidGround == 0;
					}
				}
				else {
					isOnSolidGround == 1;
				}
				//if the left button is pressed AND we're still right of the left wall
				//move the box left by 4 pixels
				if (key[2] && square_x >= 4) {
					velocity_vx = -8;
				}

				//if the left button is pressed AND we're still left of the right wall
				//move the box right by 4 pixels
				if (key[3] && square_x <= WorldWidth - 32) {
					velocity_vx = 8;
				}

				//pseudocde for stopping forward movement on jump
				//check if key is FALSE
				//if false, set x velocity to 0
				

				//Code for later use
				//int x = object.x - cameraX;
				//int y = object.y - cameraY;

				//make camera follow the player
				cameraX = square_x - 640 / 2;
				cameraY = square_y - 480 / 2;

				if (cameraX < 0)
					cameraX = 0;
				if (cameraY < 0)
					cameraY = 0;
				if (cameraX > WorldWidth - screen_W)
					cameraX = WorldWidth - screen_W;
				if (cameraY > WorldHeight - screen_H)
					cameraY = WorldHeight - screen_H;


				if (isOnSolidGround == 0) {
					velocity_vy += .5;
				}



				//if you're on the ground (or lower) set isOnSolidGround to TRUE
				if (square_y > 480 - 72) {
					square_y = 480 - 72;
					velocity_vy = 0;
					isOnSolidGround = 1;
				}
				//otherwise you're NOT on solid ground
				else {
					isOnSolidGround = 0;
				}




				square_x += velocity_vx;
				square_y += velocity_vy;


				//stop the character from walking off the screen to the left
				if (square_x < 0 && isOnSolidGround == 0) {
					square_x = 0;
				}
				else if (square_x > 5000 && isOnSolidGround == 0) {
					state = 3;
					square_x = 50;
					square_y = 50;
				}
			}//end state MAP


			if (state == MAP2) {
				


				if (isOnSolidGround) {
					velocity_vx = .00;
					if (key[0]) {
						velocity_vy = -10;
						isOnSolidGround == 0;
					}
				}
				else {
					isOnSolidGround == 1;
				}
				//if the left button is pressed AND we're still right of the left wall
				//move the box left by 4 pixels
				if (key[2] && square_x >= 4) {
					velocity_vx = -4;
				}

				//if the left button is pressed AND we're still left of the right wall
				//move the box right by 4 pixels
				if (key[3] && square_x <= WorldWidth - 32) {
					velocity_vx = 4;
				}

				//Code for later use
				//int x = object.x - cameraX;
				//int y = object.y - cameraY;

				//make camera follow the player
				cameraX = square_x - 640 / 2;
				cameraY = square_y - 480 / 2;

				if (cameraX < 0)
					cameraX = 0;
				if (cameraY < 0)
					cameraY = 0;
				if (cameraX > WorldWidth - screen_W)
					cameraX = WorldWidth - screen_W;
				if (cameraY > WorldHeight - screen_H)
					cameraY = WorldHeight - screen_H;


				if (isOnSolidGround == 0) {
					velocity_vy += .5;
				}



				//if you're on the ground (or lower) set isOnSolidGround to TRUE
				if (square_y > 480 - 72) {
					square_y = 480 - 72;
					velocity_vy = 0;
					isOnSolidGround = 1;
				}
				//otherwise you're NOT on solid ground
				else {
					isOnSolidGround = 0;
				}




				square_x += velocity_vx;
				square_y += velocity_vy;


				//stop the character from walking off the screen to the left
				if (square_x < 0 && isOnSolidGround == 0) {
					square_x = 0;
				}
				else if (square_x > 5000 && isOnSolidGround == 0) {
					square_x = 50;
					square_y = 50;
					state = 4;
				}
			}//end state MAP2


			if (state == MAP3) {



				if (isOnSolidGround) {
					velocity_vx = .00;
					if (key[0]) {
						velocity_vy = -10;
						isOnSolidGround == 0;
					}
				}
				else {
					isOnSolidGround == 1;
				}
				//if the left button is pressed AND we're still right of the left wall
				//move the box left by 4 pixels
				if (key[2] && square_x >= 4) {
					velocity_vx = -4;
				}

				//if the left button is pressed AND we're still left of the right wall
				//move the box right by 4 pixels
				if (key[3] && square_x <= WorldWidth - 32) {
					velocity_vx = 4;
				}

				//Code for later use
				//int x = object.x - cameraX;
				//int y = object.y - cameraY;

				//make camera follow the player
				cameraX = square_x - 640 / 2;
				cameraY = square_y - 480 / 2;

				if (cameraX < 0)
					cameraX = 0;
				if (cameraY < 0)
					cameraY = 0;
				if (cameraX > WorldWidth - screen_W)
					cameraX = WorldWidth - screen_W;
				if (cameraY > WorldHeight - screen_H)
					cameraY = WorldHeight - screen_H;


				if (isOnSolidGround == 0) {
					velocity_vy += .5;
				}



				//if you're on the ground (or lower) set isOnSolidGround to TRUE
				if (square_y > 480 - 72) {
					square_y = 480 - 72;
					velocity_vy = 0;
					isOnSolidGround = 1;
				}
				//otherwise you're NOT on solid ground
				else {
					isOnSolidGround = 0;
				}




				square_x += velocity_vx;
				square_y += velocity_vy;


				//stop the character from walking off the screen to the left
				if (square_x < 0 && isOnSolidGround == 0) {
					square_x = 0;
				}
				else if (square_x > 5000 && isOnSolidGround == 0) {
					square_x = 5000;
					state = 3;
				}
			}//end state MAP3
			//redraw at every tick of the timer
			redraw = true;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the algorithm that turns key presses into events
		//a "key down" event is when a key is pushed
		//while a "key up" event is when a key is released

		//has something been pressed on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN/* ||
												  ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY*/) {
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;

				//if the left key has been pressed
			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				break;

				//if the right key has been pressed
			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				break;
			case ALLEGRO_KEY_ENTER:
				key[4] = true;
				break;
			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[2] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[3] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				key[4] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

				//kill the program if someone presses escape
				//case ALLEGRO_KEY_ESCAPE:
				//ESC = true;
				//break;
			}
		}

		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algo rithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			if (state == MAP) {
				al_play_sample_instance(instance1);


				al_draw_bitmap_region(background, cameraX, cameraY, 640, 480, 0, 0, 0);
				al_draw_bitmap(square, square_x - cameraX, square_y - cameraY, 0);


				//al_draw_bitmap_region(square, square_x - cameraX, square_y - cameraY,640,480,0,0,0);
			}
			if (state == MAP2) {
				

				al_draw_bitmap_region(background2, cameraX, cameraY, 640, 480, 0, 0, 0);
				al_draw_bitmap(square, square_x - cameraX, square_y - cameraY, 0);
				
			}
			if (state == MAP3) {



				al_draw_bitmap_region(background3, cameraX, cameraY, 640, 480, 0, 0, 0);
				al_draw_bitmap(square, square_x - cameraX, square_y - cameraY, 0);
			}
			if (state == INTRO) {
				al_draw_text(font, al_map_rgb(255, 0, 0), 100, 100, 0, "I hate my life");
			}

			al_flip_display();
		}
	}
	al_destroy_sample(sample1);
	al_destroy_sample_instance(instance1);
	al_destroy_bitmap(square);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}