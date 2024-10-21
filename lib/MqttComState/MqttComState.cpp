#include "MqttComState.h"

void initStateText()
{
    stateText[0]='\0';
}

void setStateText(char *text)
{
    for(int i = 0; text[i]!='\0';i++)
    {
        stateText[i]=text[i];
    }
}

char *getStateText()
{
    return stateText;
}

void initStateArray()
{
    for(int i = 0;i< MAX_SIZE;i++)
    {
        stateArray[i] = SWITCH_OFF;
    }
}

void setState(int port, int state)
{
    stateArray[port] = state;
}

int getState(int port)
{
    return stateArray[port];
}