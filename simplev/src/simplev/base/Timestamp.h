/*
 * Timestamp.h
 *
 *  Created on: Jun 19, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_TIMESTAMP_H_
#define SIMPLEV_BASE_TIMESTAMP_H_

#include <algorithm>
#include <string>

#include <simplev/base/copyable.h>

namespace simplev
{

class Timestamp: public simplev::copyable
{
public:
	Timestamp() :
			secondsSinceEpoch_(0)
	{
	}

	explicit Timestamp(double secondsSinceEpoch) :
			secondsSinceEpoch_(secondsSinceEpoch)
	{
	}

	std::string toFormattedString() const;

	// Get time of now.
	static Timestamp now();

  // for internal usage.
  double secondsSinceEpoch() const { return secondsSinceEpoch_; }

private:
	double secondsSinceEpoch_;
};
}

#endif /* SIMPLEV_BASE_TIMESTAMP_H_ */
