# Smart-RGB-LED-strip-dual-controlled-using-Lumi-Life-App-and-physical-control-board
- PROJECT: Smart RGB LED strip dual-controlled using Lumi Life App & physical control board
  
- Here is the demo video: https://drive.google.com/file/d/15P0fPUeadqqbs1FvRIRN9EnKdWg_IYVA/view?usp=sharing

- WIRING:
	- Control board:
		- STM32F401RE  PC6		----UART---->    PC0			EFR32MG21
	- LED strip connection:
		- WS2812       PC0 (Pulled UP 5V) --One-Wire-->    PC0 (Pulled UP 5V)	EFR32MG21
		- WS2812       GND 		------------>    GND			EFR32MG21
	   	- WS2812       5V		------------>    5V Supply
	  	- EFR32MG24    5V		------------>    5V Supply
 
- BINDING GUIDE:
	- Press any button (SW1 / SW2) on the EFR32MG21 kit (LED strip) twice to start as a Binding Target.
	- Wait till the LED starts blinking PINK.
	- Press any button (SW1 / SW2) on the EFR32MG21 kit (Control Board) twice to start as a Binding Initiator.
	- Wait till the LED starts blinking PINK.

- NETWORK JOIN / LEAVE
	- The devices will automatically steer for a network when NETWORK_DOWN.
	- Upon finding a network, the LED will start blinking PINK
	- Press the SW2 button on the EFR32MG21 kit (Control Board & LED Strip) 5 times to leave the network and reset (the LED starts blinking RED).

- CONTROL BOARD GUIDE:
	- Use B1 and B5 buttons to navigate options.
	- Use B3 to Submit / Move to the next screen.
	- Use B2 and B4 buttons to increment/decrement (RGB value, Time value).
	- If any error occurs, press the reset button under the board, and necessary data (time, RGB value, room) will be permanently saved.
