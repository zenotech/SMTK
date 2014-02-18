#include "smtk/model/ModelEntity.h"

#include "smtk/model/CellEntity.h"
#include "smtk/model/GroupEntity.h"
#include "smtk/model/Storage.h"
#include "smtk/model/Arrangement.h"

namespace smtk {
  namespace model {

Cursor ModelEntity::parent() const
{
  return CursorArrangementOps::firstRelation<Cursor>(*this, EMBEDDED_IN);
}

/// Return the cells directly owned by this model.
CellEntities ModelEntity::cells() const
{
  CellEntities result;
  CursorArrangementOps::appendAllRelations(*this, INCLUDES, result);
  return result;
}

/// Return the groups directly owned by this model.
GroupEntities ModelEntity::groups() const
{
  GroupEntities result;
  CursorArrangementOps::appendAllRelations(*this, SUPERSET_OF, result);
  return result;
}

/// Return the models directly owned by this model.
ModelEntities ModelEntity::submodels() const
{
  ModelEntities result;
  CursorArrangementOps::appendAllRelations(*this, SUPERSET_OF, result);
  return result;
}

ModelEntity& ModelEntity::addCell(const CellEntity& c)
{
  this->embedEntity(c);
  return *this;
}

ModelEntity& ModelEntity::addGroup(const GroupEntity& g)
{
  if (this->isValid() && g.isValid())
    {
    CursorArrangementOps::findOrAddSimpleRelationship(*this, SUPERSET_OF, g);
    CursorArrangementOps::findOrAddSimpleRelationship(g, SUBSET_OF, *this);
    this->m_storage->trigger(ADD_GROUP_TO_MODEL, *this, g);
    }
  return *this;
}

ModelEntity& ModelEntity::addSubmodel(const ModelEntity& m)
{
  if (this->isValid() && m.isValid() && m.storage() == this->storage() && m.entity() != this->entity())
    {
    CursorArrangementOps::findOrAddSimpleRelationship(*this, SUPERSET_OF, m);
    CursorArrangementOps::findOrAddSimpleRelationship(m, SUBSET_OF, *this);
    this->m_storage->trigger(ADD_MODEL_TO_MODEL, *this, m);
    }
  return *this;
}

  } // namespace model
} // namespace smtk
