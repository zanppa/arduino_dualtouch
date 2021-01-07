/*
 * DualTouch.h -- Simple library to handle 2 touch buttons
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

#ifndef __DUALTOUCH_H__
#define __DUALTOUCH_H__

class DualTouch
{
public:
  DualTouch(uint8_t leftPin, uint8_t rightPin);

  void setThreshold(float threshold);
  void setInvert(bool invert);
  void setFilterGains(float lowpassGain, float threholdGain);
  void setReadTimes(uint8_t times);
  void setReadInterval(uint16_t interval);

  bool leftTouched();
  bool rightTouched();

  void update(void);

private:
  uint8_t pin_1;
  uint8_t pin_2;
  uint8_t readTimes;
  uint16_t readInterval;
  bool invert;
  
  uint16_t value1;
  uint16_t value2;

  // Threshold for triggering touch
  float threshold;

  // Low pass filtered values
  float gain;
  float invgain;
  float value1_f;
  float value2_f;

  // Threshold values
  float gain_th;
  float invgain_th;
  float threshold_1;
  float threshold_2;

  // States
  bool state_1;
  bool state_2;

  void readSensors(void);
  void updateStatus(void);
  void resetStatus(void);
};

#endif // __DUALTOUCH_H__
