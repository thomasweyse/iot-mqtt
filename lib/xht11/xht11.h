/**
   ���֣�xht11.h
   ���ܣ�ʵ���Զ����ͷ�ļ�
   ������arduino, esp32, raspberry pi pico
   ���ߣ�Dengfei
   ʱ�䣺2021/1/5
*/

#ifndef _xht11_H__
#define _xht11_H__

#include "Arduino.h"

class xht11
{
  private:
    int XHT11_pin ;

  public:
    xht11(int pin);
    void start();
    unsigned char rec_byte();
    bool receive(unsigned char *xht);
};

#endif
