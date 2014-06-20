/*
 * Timestamp.cc
 *
 *  Created on: Jun 20, 2014
 *      Author: damonhao
 */
#include <simplev/base/Timestamp.h>

#include <stdio.h>
#include <sys/time.h>

using namespace simplev;

std::string Timestamp::toFormattedString() const
{

  char buf[32] = {0};
  time_t seconds = static_cast<time_t>(static_cast<int>(secondsSinceEpoch_));

  int microseconds = static_cast<int>((secondsSinceEpoch_ - static_cast<int>(secondsSinceEpoch_)) * 1000000);
  struct tm tm_time;
  gmtime_r(&seconds, &tm_time);

  snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
      tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
      tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
      microseconds);
  return buf;

}



