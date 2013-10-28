#include "smtk/model/Link.h"

#include <algorithm>
#include <set>
#include <map>
#include <vector>

#include <sstream>

//#include <boost/variant.hpp>

using namespace std;
using namespace smtk::util;

namespace smtk {
  namespace model {

/**\class smtk::model::Link - Store links between named entities.
  */

/// The default constructor creates an invalid link.
Link::Link()
  : EntityFlags(INVALID), Dimension(-1)
{
}

/// Construct a link with the given \a dimension with a type specified by \a entityFlags.
Link::Link(int entityFlags, int dimension)
  : EntityFlags(entityFlags), Dimension(dimension)
{
}

/**\brief Return the bit vector describing the type and attributes of the associated entity.
  *
  * \sa smtk::model::EntityTypeBits
  */
int Link::entityFlags() const
{
  return this->EntityFlags;
}

/**\brief Return the dimension of the associated entity.
  *
  * When \a entityFlags() includes the CELL bit, this must be in [0,3] for
  * the cell to be considered valid.
  * For the SHELL bit, this must be in [1,3] for the shell to be valid.
  * For the USE bit, this must be in [0,2] for the use to be valid.
  *
  * For the GROUP bit, the integer returned should be treated as a bit
  * vector. Valid values include [0,15].
  */
int Link::dimension() const
{
  return this->Dimension;
}

UUIDArray& Link::relations()
{
  return this->Relations;
}
const UUIDArray& Link::relations() const
{
  return this->Relations;
}

Link& Link::appendRelation(const UUID& b)
{
  this->Relations.push_back(b);
  return *this;
}

Link& Link::removeRelation(const UUID& b)
{
  UUIDArray& arr(this->Relations);
  unsigned size = arr.size();
  unsigned curr;
  for (curr = 0; curr < size; ++curr)
    {
    if (arr[curr] == b)
      {
      arr.erase(arr.begin() + curr);
      --curr;
      --size;
      }
    }
  return *this;
}

  } // namespace model
} //namespace smtk