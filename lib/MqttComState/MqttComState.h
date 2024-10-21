#define MAX_SIZE    32

#define SWITCH_OFF  0
#define SWITCH_ON   1

#define ROOF_LED    0
#define WINDOW      1
#define DOOR        2
#define VENTILATOR  3
#define GAS         4

// TODO add further states for every service (door, window)

int stateArray[MAX_SIZE];

char stateText[MAX_SIZE];
