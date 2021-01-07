/*
 * DualTouch.cpp -- Simple library to handle 2 touch buttons
 * Copyright (C) 2020 Lauri Peltonen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
 
#include <arduino.h>
#include "DualTouch.h"

DualTouch::DualTouch(uint8_t leftPin, uint8_t rightPin)
{
  this->pin_1 = leftPin;
  this->pin_2 = rightPin;

  // Set all default values
  this->readTimes = 40;
  this->readInterval = 1000;  // 1 ms
 
  // Threshold for triggering touch
  this->threshold = 0.05;

  // Low pass filtered values
  this->gain = 0.25;
  this->invgain = 1.0 - this->gain;
  this->value1_f = -1.0;
  this->value2_f = -1.0;

  this->gain_th = 0.03;  // Threshold low pass filter
  this->invgain_th = 1.0 - this->gain_th;
  this->threshold_1 = 5000.0;
  this->threshold_2 = 5000.0;

  this->invert = false;

  // States
  this->state_1 = false;
  this->state_2 = false;

  this->value1 = 0;
  this->value2 = 0;
}

void DualTouch::setThreshold(float threshold)
{
  this->threshold = threshold;
}

void DualTouch::setFilterGains(float lowpassGain, float thresholdGain)
{
  this->gain = lowpassGain;
  this->invgain = 1.0 - lowpassGain;
  this->gain_th = thresholdGain;
  this->invgain_th = 1.0 - thresholdGain;
}

void DualTouch::setReadTimes(uint8_t times)
{
  if(times > 64) times = 64;  // Prevent overflow
  this->readTimes = times;
}

void DualTouch::setReadInterval(uint16_t interval)
{
  this->readInterval = interval;
}

bool DualTouch::leftTouched()
{
  return this->state_1;
}

bool DualTouch::rightTouched()
{
  return this->state_2;
}

void DualTouch::update(void)
{
  this->readSensors();
  this->updateStatus();
}

void DualTouch::setInvert(bool invert)
{
  this->invert = invert;
}

void DualTouch::readSensors(void)
{
  this->value1 = 0;
  this->value2 = 0;
  
  for(uint8_t i=0; i<readTimes + 1; ++i) {
    long start = micros();

    // Left sensor
    pinMode(this->pin_1, OUTPUT);
    digitalWrite(this->pin_1, HIGH);
    pinMode(this->pin_2, OUTPUT);
    digitalWrite(this->pin_2, LOW);

    pinMode(this->pin_1, INPUT);

    uint16_t temp = analogRead(pin_1);
    if(i) this->value1 += temp;
    
    // Right
    pinMode(this->pin_1, OUTPUT);
    digitalWrite(this->pin_1, LOW);
    pinMode(this->pin_2, OUTPUT);
    digitalWrite(this->pin_2, HIGH);

    pinMode(this->pin_2, INPUT);

    temp = analogRead(pin_2);
    if(i) this->value2 += temp;

    while(micros() - start < this->readInterval);
  }
}

void DualTouch::updateStatus(void)
{
  if (value1_f < 0)
    this->resetStatus();

  // Low pass filter the values for threshold calculation
  float fvalue1 = (float)this->value1;
  float fvalue2 = (float)this->value2;
  this->value1_f = this->gain * fvalue1 + this->invgain * value1_f;
  this->value2_f = this->gain * fvalue2 + this->invgain * value2_f;

  float act_th1 = 1.0 + (this->state_1 ? -this->threshold : this->threshold);
  float act_th2 = 1.0 + (this->state_2 ? -this->threshold : this->threshold);
  this->threshold_1 = this->gain_th * fvalue1 * act_th1 + this->invgain_th * threshold_1;
  this->threshold_2 = this->gain_th * fvalue2 * act_th2 + this->invgain_th * threshold_2;

  if (this->value1_f > this->threshold_1) this->state_1 = true;
  else this->state_1 = false;
  if(this->invert) this->state_1 = !this->state_1;

  if (this->value2_f > this->threshold_2) this->state_2 = true;
  else this->state_2 = false;
  if(this->invert) this->state_2 = !this->state_2;
}

void DualTouch::resetStatus(void)
{
  this->state_1 = false;
  this->state_2 = false;
  this->value1_f = (float)value1;
  this->value2_f = (float)value2;
  
  float th_up = 1.0 + 4.0 * this->threshold;  // A bit high to prevent noise from triggering this
  this->threshold_1 = this->value1_f * th_up;
  this->threshold_2 = this->value2_f * th_up;
}
