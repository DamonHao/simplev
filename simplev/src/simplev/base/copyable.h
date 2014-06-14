/*
 * copyable.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_COPYABLE_H_
#define SIMPLEV_BASE_COPYABLE_H_

namespace simplev
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable
{
};

}

#endif /* SIMPLEV_BASE_COPYABLE_H_ */
