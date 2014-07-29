#include "smtk/model/DefaultBridge.h"

#include "smtk/model/BRepModel.h"
#include "smtk/model/RemoteOperator.h"

#include "smtk/attribute/Attribute.h"
#include "smtk/attribute/IntItem.h"

#include "smtk/util/AutoInit.h"

namespace smtk {
  namespace model {

/// Default constructor. Initializes statically-registered operators.
DefaultBridge::DefaultBridge()
{
  this->m_importingOperators = false;
  this->initializeOperatorManager(DefaultBridge::s_operators);
}

/// Indicate that, since we have no "backing store" model, the entire model is already present.
BridgedInfoBits DefaultBridge::transcribeInternal(const Cursor& entity, BridgedInfoBits flags)
{
  (void)entity;
  (void)flags;
  return BRIDGE_EVERYTHING;
}

/**\brief Call this method to indicate that the bridge acts
  *       as a backing store for a remote session.
  *
  * Some applications built on SMTK need to present information about
  * a model on a client that is remote to the geometric modeling kernel.
  * This call indicates that the bridge should be used to locally mirror
  * operations on the server.
  *
  * All this does is set the bridge's name and session ID to match
  * the remote bridge.
  * Subclasses are responsible for overriding transcribeInternal() to
  * fetch records from the remote session's model manager on demand.
  *
  * It is most likely, in addition to subclassing this bridge to
  * use your application's client-server communication mechanism,
  * you will also wish to subclass RemoteOperator.
  */
void DefaultBridge::backsRemoteBridge(
  const std::string& remoteBridgeName,
  const smtk::util::UUID& bridgeSessionId)
{
  this->m_remoteBridgeName = remoteBridgeName;
  this->m_sessionId = bridgeSessionId;
}

/**\brief Returns an empty string or, when backsRemoteBridge
  *       has been called, the type-name of the remote bridge.
  *
  */
std::string DefaultBridge::remoteName() const
{
  return this->m_remoteBridgeName;
}

/**\brief Return an instance of the operator of the given name, if it exists.
  *
  * Under some circumstances, a RemoteOperator will be created and returned:
  * 1. No existing operator matches the given name.
  * 2. The remoteName() method returns a non-empty string (i..e, backsRemoteBridge
  *    has been called).
  * 3. A friend class (such as ImportJSON) has called setImportingOperators(true)
  *    and not subsequently called setImportingOperators(false).
  */
OperatorPtr DefaultBridge::op(const std::string& opName, ManagerPtr manager) const
{
  OperatorPtr oper = this->Bridge::op(opName, manager);
  if (this->m_importingOperators && !oper && !this->m_remoteBridgeName.empty())
    { // we are a remote bridge... create any operator our friend classes ask for.
    RemoteOperatorPtr rop = RemoteOperator::create();
    rop->setName(opName);
    rop->setManager(manager);
    // Naughty, but necessary so we can pretend that the
    // operator existed all along.
    DefaultBridge* self = const_cast<DefaultBridge*>(this);
    rop->setBridge(self);
    oper = rop;
    }
  return oper;
}

/**@name RemoteOperator delegate methods.
  *
  * If this bridge has any operators derived from RemoteOperator,
  * those operators will call these methods when their ableToOperate()
  * or operate() members are invoked.
  * It is the bridge's responsibility to forward
  * the requests to the appropriate, non-virtual bridge for
  * execution and return the results.
  */
///@{
/**\brief A delegate for the RemoteOperator::ableToOperate() method.
  *
  * The DefaultBridge implementation does nothing.
  * Subclasses must override this method.
  */
bool DefaultBridge::ableToOperateDelegate(RemoteOperatorPtr oper)
{
  (void)oper;
  return false;
}

/**\brief A delegate for the RemoteOperator::operate() method.
  *
  * The DefaultBridge implementation does nothing.
  * Subclasses must override this method.
  */
OperatorResult DefaultBridge::operateDelegate(RemoteOperatorPtr oper)
{
  if (!oper)
    return OperatorResult();

  return oper->createResult(OPERATION_FAILED);
}
///@}

/// Called by friends of DefaultBridge to indicate that new RemoteOperators should be created.
void DefaultBridge::setImportingOperators(bool isImporting)
{
  this->m_importingOperators = isImporting;
}

  } // namespace model
} // namespace smtk

static const char* DefaultBridgeFileTypes[] = {
  ".json",
  NULL
};
smtkImplementsModelingKernel(native,DefaultBridgeFileTypes,smtk::model::DefaultBridge);
