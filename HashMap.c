/*******************************************************************************
* Hashmap for Substation Systems Database
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: Hashmap Data Structure
*******************************************************************************/

struct telemetry_point {
	/*the location of the substation*/
	char[] location; 
	/*Unique code for each piece of equipment as an identifier*/
	char[] desig; 
	/*What type of equipment, eg. Transformer, circuit breaker*/
	char[] plant; 
	/*What container does this equipment belong to, eg.voltage level*/ 
	char[] network; 
	/*The specific point which we log, eg. oil temperature*/
	char[] quantity; 
	/*The communications protocol used to talk to equip*/
	char[] protocol; 
	/*The channel number of the comms module talking to equip*/
	int number; 
	/*The module which holds the channels address*/
	int address; 
	/*What type of signal is on the module, eg Dig, Ana*/
	char[] moduletype; 
	/*=1 if communicating ok when the CSV was saved, =0 if failed*/
	int failed; 
	/*Specifies whether the point was online when the CSV was saved,
	(we can set certain points offline if they are causing problems 
	or not required to avoid alarms, events etc.)*/
	int online; 
	/*Same as failed, not sure why we use this at work*/
	int faulty; 
	/* Out of service */
	int oos; 
} typedef struct telemetry_point telemetry_point_t;



/* Still need to do more learning on this - Michael */