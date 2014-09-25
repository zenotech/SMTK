
set(RemoteLibSrcs
  RemusBridgeConnection.cxx
  RemusBridgeConnections.cxx
  RemusRemoteBridge.cxx
)

set(RemoteLibHeaders
  RemusBridgeConnection.h
  RemusBridgeConnections.h
  RemusRemoteBridge.h
)

add_library(SMTKRemote ${RemoteLibSrcs})

# Install the headers
smtk_public_headers(${RemoteLibHeaders})

#set SMTKRemote to publicly link to SMTKCore and privately to Remus
target_link_libraries(SMTKRemote
  LINK_PUBLIC
    SMTKCore
  LINK_PRIVATE
    RemusCommon
    RemusClient
  )

target_include_directories(SMTKRemote
  PUBLIC
  ${REMUS_INCLUDE_DIRS}
  )

if (SMTK_BUILD_CGM)
  target_include_directories(SMTKRemote PRIVATE ${CGM_INCLUDE_DIRS})
endif()

smtk_export_header(SMTKRemote SMTKRemoteExports.h)

# On Mac OS X, set the directory included as part of the installed library's path:
if (BUILD_SHARED_LIBS)
  set_target_properties(SMTKRemote PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#install the library and exports the library when used from a build tree
smtk_install_library(SMTKRemote DEPENDS SMTKCore)

# Add a server that accepts arguments for locations to find workers.
add_executable(smtk-model-server
  RemusModelServer.cxx
)

target_link_libraries(smtk-model-server
  SMTKCore
  RemusCommon
  RemusServer
)
target_include_directories(smtk-model-server PRIVATE ${REMUS_INCLUDE_DIRS})

# Add a worker that links to each modeling kernel we bridge
add_executable(smtk-remote-model
  smtk-remote-model.cxx
  RemusRPCWorker.cxx
)

target_link_libraries(smtk-remote-model
  SMTKCore
  SMTKRemote
  RemusCommon
  RemusProto
  RemusClient
  RemusWorker
)
target_include_directories(smtk-remote-model PRIVATE ${REMUS_INCLUDE_DIRS})
if (SMTK_BUILD_CGM)
  target_link_libraries(smtk-remote-model cgmSMTK)
  target_include_directories(smtk-remote-model PRIVATE ${CGM_INCLUDE_DIRS})
endif()

if (SMTK_ENABLE_TESTING)
  # ... and make header compilation tests link properly:
  target_link_libraries(TestBuild_smtk_bridge_remote LINK_PRIVATE RemusCommon)
  target_include_directories(TestBuild_smtk_bridge_remote PRIVATE ${REMUS_INCLUDE_DIRS})

  add_subdirectory(testing)
endif()