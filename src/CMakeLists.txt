add_executable(startAll main.cpp UserProcess.cpp jMessageQueue.cpp ResourceManager.cpp ConfigParser.cpp jLock.cpp jSharesMemory.cpp AbstractProcess.cpp PageAllocationServer.cpp PageSubstitutionServer.cpp)
target_link_libraries(startAll -lrt -pthread)
