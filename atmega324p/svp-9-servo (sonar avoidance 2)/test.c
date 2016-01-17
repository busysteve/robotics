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

int f_back = 000;
int f_forward = 800;

int f_up = 100;
int f_down = 600;

int b_back = 000;
int b_forward = 800;

int b_up = 100;
int b_down = 600;

int wait = 350;

// Custom limits for this bot's servos' ranges
int servo_limits [][2] = 
	{ 
		{ 1900, 1100 },  // Servo 0
		{ 1000, 1800 },  // Servo 1
		{ 1900, 800  },  // Servo 2
		{ 1000, 1800 },  // Servo 3
		{ 1100, 2200 },  // Servo 4
		{ 1700, 700  },  // Servo 5
		{ 1300, 2300 },  // Servo 6
		{ 1900, 800  }   // Servo 7
	};

/*		
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
*/		

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


int sonar_ping( int trigger_pin, int echo_pin )
{
	//set_digital_input( IO_D2, LOW );
	set_digital_input(IO_D2, PULL_UP_ENABLED);
	int lms = 0;
	int hms = 0;
	int i;

	for( i=0; i < 3; i++ ) // try times to get a none negative value
	{
		time_reset();
		int ts = ticks_to_microseconds(get_ticks());

		set_digital_output(IO_D1, LOW);			
		delay_us(15);
		set_digital_output(IO_D1, HIGH);			
		delay_us(15);
		set_digital_output(IO_D1, LOW);			
	
		while( !is_digital_input_high(IO_D2) )
		{
			lms = ticks_to_microseconds(get_ticks());

			if( lms > (ts+8000) )
			{
				break;
			}
		}

		while( is_digital_input_high(IO_D2) )
		{
			hms = ticks_to_microseconds(get_ticks());

			if( hms > (lms+8000) )
			{
				break;
			}
		}

		if( (hms - lms) >= 0 )
			break;
	}
	
	int ret = (hms - lms);

	if( ret < 0 )
		ret = 0;

	return ret;

} 

int sonar_cm( int usecs )
{
	return (usecs / 59);
}

int sonar_in( int usecs )
{
	return (usecs / 145);
}


void step_forward_right()
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

	change_wait( &wait ); delay_ms( wait );

	servo_move( FRONT_RIGHT_LEG,  f_forward, forward_wait );
	servo_move( REAR_LEFT_LEG,  b_forward, forward_wait );

	servo_move( FRONT_LEFT_LEG,  f_back, back_wait );
	servo_move( REAR_RIGHT_LEG,  b_back, back_wait );

	change_wait( &wait ); delay_ms( wait );
}

void step_forward_left()
{
	int up_wait = 0;
	int forward_wait = 0;
	int down_wait = 0;
	int back_wait = 0;

	// -------------------
	servo_move( REAR_RIGHT_KNEE, b_up, up_wait );
	servo_move( FRONT_LEFT_KNEE, f_up, up_wait );

	servo_move( FRONT_RIGHT_KNEE, f_down, down_wait );
	servo_move( REAR_LEFT_KNEE, b_down, down_wait );

	change_wait( &wait ); delay_ms( wait );

	servo_move( REAR_RIGHT_LEG,  b_forward, forward_wait );
	servo_move( FRONT_LEFT_LEG,  f_forward, forward_wait );

	servo_move( FRONT_RIGHT_LEG, f_back, back_wait );
	servo_move( REAR_LEFT_LEG,   b_back, back_wait );

	change_wait( &wait ); delay_ms( wait );

}

void step_backward_left()
{
	int up_wait = 0;
	int forward_wait = 0;
	int down_wait = 0;
	int back_wait = 0;

	servo_move( FRONT_RIGHT_KNEE, f_down, up_wait );
	servo_move( REAR_LEFT_KNEE, b_down, up_wait );

	servo_move( REAR_RIGHT_KNEE, b_up, down_wait );
	servo_move( FRONT_LEFT_KNEE, f_up, down_wait );

	change_wait( &wait ); delay_ms( wait );

	servo_move( FRONT_RIGHT_LEG,  f_forward, forward_wait );
	servo_move( REAR_LEFT_LEG,  b_forward, forward_wait );

	servo_move( FRONT_LEFT_LEG,  f_back, back_wait );
	servo_move( REAR_RIGHT_LEG,  b_back, back_wait );

	change_wait( &wait ); delay_ms( wait );
}

void step_backward_right()
{
	int up_wait = 0;
	int forward_wait = 0;
	int down_wait = 0;
	int back_wait = 0;

	servo_move( REAR_RIGHT_KNEE, b_down, up_wait );
	servo_move( FRONT_LEFT_KNEE, f_down, up_wait );

	servo_move( FRONT_RIGHT_KNEE, f_up, down_wait );
	servo_move( REAR_LEFT_KNEE, b_up, down_wait );

	change_wait( &wait ); delay_ms( wait );

	servo_move( REAR_RIGHT_LEG,  b_forward, forward_wait );
	servo_move( FRONT_LEFT_LEG,  f_forward, forward_wait );

	servo_move( FRONT_RIGHT_LEG, f_back, back_wait );
	servo_move( REAR_LEFT_LEG,   b_back, back_wait );

	change_wait( &wait ); delay_ms( wait );
}

void turn_right()
{
				int up_wait = 0;
				int forward_wait = 0;
				int down_wait = 0;
				int back_wait = 0;


				// -------------------
				servo_move( FRONT_LEFT_KNEE, f_up, up_wait );
				servo_move( REAR_RIGHT_KNEE, b_up, up_wait );

				servo_move( REAR_LEFT_KNEE, b_down, down_wait );
				servo_move( FRONT_RIGHT_KNEE, f_down, down_wait );

				change_wait( &wait ); delay_ms( wait );

				servo_move( FRONT_LEFT_LEG,  f_back, forward_wait );
				servo_move( REAR_RIGHT_LEG,  b_forward, forward_wait );

				servo_move( FRONT_RIGHT_LEG,  f_back, back_wait );
				servo_move( REAR_LEFT_LEG,  b_forward, back_wait );

				change_wait( &wait ); delay_ms( wait );


				// -------------------
				// -------------------
				servo_move( REAR_LEFT_KNEE, b_up, down_wait );
				servo_move( FRONT_RIGHT_KNEE, f_up, down_wait );

				servo_move( FRONT_LEFT_KNEE, f_down, up_wait );
				servo_move( REAR_RIGHT_KNEE, b_down, up_wait );


				change_wait( &wait ); delay_ms( wait );

				servo_move( FRONT_LEFT_LEG,  f_forward, forward_wait );
				servo_move( REAR_RIGHT_LEG,  b_back, forward_wait );

				servo_move( FRONT_RIGHT_LEG,  f_forward, back_wait );
				servo_move( REAR_LEFT_LEG,  b_back, back_wait );


				change_wait( &wait ); delay_ms( wait );

				// -------------------
}

// Turning Left
void turn_left()
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

	change_wait( &wait ); delay_ms( wait );

	servo_move( FRONT_RIGHT_LEG,  f_back, forward_wait );
	servo_move( REAR_LEFT_LEG,  b_forward, forward_wait );

	servo_move( FRONT_LEFT_LEG,  f_back, back_wait );
	servo_move( REAR_RIGHT_LEG,  b_forward, back_wait );

	change_wait( &wait ); delay_ms( wait );

	// -------------------
	servo_move( REAR_RIGHT_KNEE, b_up, down_wait );
	servo_move( FRONT_LEFT_KNEE, f_up, down_wait );

	servo_move( FRONT_RIGHT_KNEE, f_down, up_wait );
	servo_move( REAR_LEFT_KNEE, b_down, up_wait );

	change_wait( &wait ); delay_ms( wait );

	servo_move( FRONT_RIGHT_LEG,  f_forward, forward_wait );
	servo_move( REAR_LEFT_LEG,  b_back, forward_wait );

	servo_move( FRONT_LEFT_LEG,  f_forward, back_wait );
	servo_move( REAR_RIGHT_LEG,  b_back, back_wait );

	change_wait( &wait ); delay_ms( wait );	
}


int sonar_dump_cm()
{

	int ping = sonar_ping( IO_D1, IO_D2 );

	clear();

	print_long( sonar_in(ping) );
	print("in : ");
	print_long( sonar_cm(ping) );
	print("cm");

	lcd_goto_xy( 0, 1 );

	print_long(read_battery_millivolts_svp());
	print(" millivolts");
	
	return sonar_cm(ping);
}




int main()
{
	// This array specifies the correspondence between I/O pins and DEMUX
	// output-selection pins.  This demo uses three pins, which allows you
	// to control up to 8 servos.  You can also use two, one, or zero pins
	// to control fewer servos.
	const unsigned char demuxPins[] = {IO_B3, IO_B4, IO_C0}; // eight servos
	//servos_start(demuxPins, sizeof(demuxPins));

	//const unsigned char servoPinsB[] = {IO_A0, IO_A1, IO_A2, IO_A3, IO_A4, IO_A5, IO_A6, IO_A7};
	const unsigned char servoPinsB[] = {IO_A0};

	servos_start_extended(demuxPins, sizeof(demuxPins), servoPinsB, sizeof(servoPinsB));

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
	set_servo_speed_b(0, 250);
	

	clear();
	print("Ready...");

	int step = 100;
	int move = 1480;
	set_servo_target_b( 0, move );





	while(1)  // Loop forever.
	{
		int i;


		delay_ms(200);
		// When the user presses the top button, execute a pre-programmed
		// sequence of servo movements.
		





		if (button_is_pressed(MIDDLE_BUTTON))
		{
			move += step;	
			clear();
			print_long( move );		
			set_servo_target_b( 0, move );
				
			/*
			servo_move( FRONT_RIGHT_LEG, f_forward, 0 );
			servo_move( FRONT_LEFT_LEG, f_forward, 0 );
			servo_move( REAR_RIGHT_LEG, b_forward, 0 );
			servo_move( REAR_LEFT_LEG, b_forward, 0 );
			servo_move( FRONT_RIGHT_KNEE, f_down, 0 );
			servo_move( FRONT_LEFT_KNEE, f_down, 0 );
			servo_move( REAR_RIGHT_KNEE, b_down, 0 );
			servo_move( REAR_LEFT_KNEE, b_down, 0 );
			*/
		}
		if (button_is_pressed(BOTTOM_BUTTON))
		{
			move -= step;
			if( move < 0 )
				move = 0;			
			clear();
			print_long( move );		
			set_servo_target_b( 0, move );

			/*
			servo_move( FRONT_RIGHT_LEG, f_back, 0 );
			servo_move( FRONT_LEFT_LEG, f_back, 0 );
			servo_move( REAR_RIGHT_LEG, b_back, 0 );
			servo_move( REAR_LEFT_LEG, b_back, 0 );
			servo_move( FRONT_RIGHT_KNEE, f_up, 0 );
			servo_move( FRONT_LEFT_KNEE, f_up, 0 );
			servo_move( REAR_RIGHT_KNEE, b_up, 0 );
			servo_move( REAR_LEFT_KNEE, b_up, 0 );
			*/
			
			

		}

		if (button_is_pressed(TOP_BUTTON))
		while(1){

			/*			
			servo_move( FRONT_RIGHT_LEG, f_forward, 0 );
			servo_move( FRONT_LEFT_LEG, f_forward, 0 );
			servo_move( REAR_RIGHT_LEG, b_forward, 0 );
			servo_move( REAR_LEFT_LEG, b_forward, 0 );
			servo_move( FRONT_RIGHT_KNEE, f_down, 0 );
			servo_move( FRONT_LEFT_KNEE, f_down, 0 );
			servo_move( REAR_RIGHT_KNEE, b_down, 0 );
			servo_move( REAR_LEFT_KNEE, b_down, 0 );
			*/

			// Walking forward
			for( i=0; i < 8; i++ )
			{
				sonar_dump_cm();
				step_forward_right();
				step_forward_left();
			}

			// Turning Right
			for( i=0; i < 3; i++ )
			{
				sonar_dump_cm();
				turn_right();
			}

			// Walking backward
			for( i=0; i < 6; i++ )
			{
				sonar_dump_cm();
				step_backward_left();	
				step_backward_right();	
			}

			// Turning Left
			for( i=0; i < 6; i++ )
			{
				sonar_dump_cm();
				turn_left();				
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
