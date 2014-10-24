//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef __smtk_bridge_discrete_MergeOperator_h
#define __smtk_bridge_discrete_MergeOperator_h

#include "smtk/bridge/cmb/discreteBridgeExports.h"
#include "smtk/model/Operator.h"
#include "vtkMergeOperator.h"
#include "vtkNew.h"

namespace smtk {
  namespace bridge {
    namespace discrete {

class Bridge;

/**\brief Merge adjacent cells into a single cell.
  *
  * The source and target cells are merged, with the result being stored in target.
  * All boundaries of the source are either modified (so that they now refer to
  * the target) or are deleted (because they served as a boundary between the source
  * and target).
  *
  * The source and target must be adjacent and have the same parametric dimension.
  */
class SMTKDISCRETEBRIDGE_EXPORT MergeOperator : public smtk::model::Operator
{
public:
  smtkTypeMacro(MergeOperator);
  smtkCreateMacro(MergeOperator);
  smtkSharedFromThisMacro(Operator);
  smtkDeclareModelOperator();

  virtual bool ableToOperate();

protected:
  MergeOperator();
  virtual smtk::model::OperatorResult operateInternal();
  Bridge* discreteBridge() const;
  int fetchCMBCellId(const std::string& parameterName) const;

  vtkNew<vtkMergeOperator> m_op;
};

    } // namespace discrete
  } // namespace bridge

} // namespace smtk

#endif // __smtk_bridge_discrete_MergeOperator_h
