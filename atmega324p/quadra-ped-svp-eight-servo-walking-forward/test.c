#include <pololu/orangutan.h>

/*
 * svp-eight-servo: for the Orangutan SVP.
 *
 * This example uses the OrangutanServos functions to control eight servos.
 * To use this example, you must connect the correct AVR I/O pins to their
 * corresponding servo demultiplexer output-selection pins.
 *   - Connect PB3 to SA.
 *   - Connect PB4 to SB.
 *   - Connect PC0 to SC.
 *
 * http://www.pololu.com/docs/0J20
 * http://www.pololu.com
 * http://forum.pololu.com
 */

#define FRONT_RIGHT_LEG   0
#define FRONT_LEFT_LEG    1
#define REAR_RIGHT_LEG    2
#define REAR_LEFT_LEG     3

#define FRONT_RIGHT_KNEE  4
#define FRONT_LEFT_KNEE   5
#define REAR_RIGHT_KNEE   6
#define REAR_LEFT_KNEE    7


// Custom limits for this bot's servos' ranges
int servo_limits [][2] = 
	{ 
		{ 2000, 1000 },  // Servo 0
		{ 900,  2100 },  // Servo 1
		{ 2100, 900  },  // Servo 2
		{ 900,  2100 },  // Servo 3
		{ 1300, 2300 },  // Servo 4
		{ 1700, 500  },  // Servo 5
		{ 1300, 2300 },  // Servo 6
		{ 2000, 800  }   // Servo 7
	};
		

void servo_move( int svo, int pos, int wait )
{	
	int diff;
	int range;
	diff = servo_limits[svo][0] - servo_limits[svo][1];

	if( diff > 0 )
	{
		range = servo_limits[svo][0] - pos;
	}
	else
	{
		range = servo_limits[svo][0] + pos;
	}

	set_servo_target( svo, range );

	delay_ms(wait);
}

void change_wait( int *wait )
{
	if (button_is_pressed(MIDDLE_BUTTON))
	{
		*wait -= 50;
		if( *wait < 0 )
			*wait = 0;
		clear();
		print_long( *wait );
	}

	if (button_is_pressed(BOTTOM_BUTTON))
	{
		*wait += 50;
		if( *wait < 0 )
			*wait = 0;
		clear();
		print_long( *wait );
	}
}

int main()
{
	// This array specifies the correspondence between I/O pins and DEMUX
	// output-selection pins.  This demo uses three pins, which allows you
	// to control up to 8 servos.  You can also use two, one, or zero pins
	// to control fewer servos.
	const unsigned char demuxPins[] = {IO_B3, IO_B4, IO_C0}; // eight servos
	//const unsigned char demuxPins[] = {IO_B3, IO_B4};      // four servos
	//const unsigned char demuxPins[] = {IO_B3};             // two servos
	//const unsigned char demuxPins[] = {};                  // one servo

	servos_start(demuxPins, sizeof(demuxPins));

	clear();

	print("Starting....");

	// Set the servo speed 
	set_servo_speed(0, 250);
	set_servo_speed(1, 250);
	set_servo_speed(2, 250);
	set_servo_speed(3, 250);
	set_servo_speed(4, 250);
	set_servo_speed(5, 250);
	set_servo_speed(6, 250);
	set_servo_speed(7, 250);
		
	clear();
	print("Ready...");

	while(1)  // Loop forever.
	{
		delay_ms(200);
		// When the user presses the top button, execute a pre-programmed
		// sequence of servo movements.
		

		int f_back = 000;
		int f_forward = 700;

		int f_up = 100;
		int f_down = 600;

		int b_back = 000;
		int b_forward = 1000;

		int b_up = 100;
		int b_down = 600;


		if (button_is_pressed(MIDDLE_BUTTON))
		{
			servo_move( FRONT_RIGHT_LEG, f_forward, 0 );
			servo_move( FRONT_LEFT_LEG, f_forward, 0 );
			servo_move( REAR_RIGHT_LEG, b_forward, 0 );
			servo_move( REAR_LEFT_LEG, b_forward, 0 );
			servo_move( FRONT_RIGHT_KNEE, f_down, 0 );
			servo_move( FRONT_LEFT_KNEE, f_down, 0 );
			servo_move( REAR_RIGHT_KNEE, b_down, 0 );
			servo_move( REAR_LEFT_KNEE, b_down, 0 );
		}
		if (button_is_pressed(BOTTOM_BUTTON))
		{
			servo_move( FRONT_RIGHT_LEG, f_back, 0 );
			servo_move( FRONT_LEFT_LEG, f_back, 0 );
			servo_move( REAR_RIGHT_LEG, b_back, 0 );
			servo_move( REAR_LEFT_LEG, b_back, 0 );
			servo_move( FRONT_RIGHT_KNEE, f_up, 0 );
			servo_move( FRONT_LEFT_KNEE, f_up, 0 );
			servo_move( REAR_RIGHT_KNEE, b_up, 0 );
			servo_move( REAR_LEFT_KNEE, b_up, 0 );
		}

		if (button_is_pressed(TOP_BUTTON))
		{
			int wait = 300;
	
			servo_move( FRONT_RIGHT_LEG, f_forward, 0 );
			servo_move( FRONT_LEFT_LEG, f_forward, 0 );
			servo_move( REAR_RIGHT_LEG, b_forward, 0 );
			servo_move( REAR_LEFT_LEG, b_forward, 0 );
			servo_move( FRONT_RIGHT_KNEE, f_down, 0 );
			servo_move( FRONT_LEFT_KNEE, f_down, 0 );
			servo_move( REAR_RIGHT_KNEE, b_down, 0 );
			servo_move( REAR_LEFT_KNEE, b_down, 0 );

			while(1)
			{
				int up_wait = 0;
				int forward_wait = 0;
				int down_wait = 0;
				int back_wait = 0;

				// -------------------
				servo_move( FRONT_RIGHT_KNEE, f_up, up_wait );
				servo_move( REAR_LEFT_KNEE, b_up, up_wait );

				servo_move( REAR_RIGHT_KNEE, b_down, down_wait );
				servo_move( FRONT_LEFT_KNEE, f_down, down_wait );

				change_wait( &wait ); delay_ms( wait / 2 );

				servo_move( FRONT_RIGHT_LEG,  f_forward, forward_wait );
				servo_move( REAR_LEFT_LEG,  b_forward, forward_wait );

				servo_move( FRONT_LEFT_LEG,  f_back, back_wait );
				servo_move( REAR_RIGHT_LEG,  b_back, back_wait );

				change_wait( &wait ); delay_ms( wait );


				// -------------------
				servo_move( REAR_RIGHT_KNEE, b_up, up_wait );
				servo_move( FRONT_LEFT_KNEE, f_up, up_wait );

				servo_move( FRONT_RIGHT_KNEE, f_down, down_wait );
				servo_move( REAR_LEFT_KNEE, b_down, down_wait );

				change_wait( &wait ); delay_ms( wait / 2);

				servo_move( REAR_RIGHT_LEG,  b_forward, forward_wait );
				servo_move( FRONT_LEFT_LEG,  f_forward, forward_wait );

				servo_move( FRONT_RIGHT_LEG, f_back, back_wait );
				servo_move( REAR_LEFT_LEG,   b_back, back_wait );

				change_wait( &wait ); delay_ms( wait );

				// -------------------
				
				
			}


		}
	}
}




// Local Variables: **
// mode: C **
// c-basic-offset: 4 **
// tab-width: 4 **
// indent-tabs-mode: t **
// end: **
