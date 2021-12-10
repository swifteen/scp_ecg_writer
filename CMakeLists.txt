project(scp_ecg_writer)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

#库输出目录 
SET(out_dir "${PROJECT_SOURCE_DIR}/build")


#add_subdirectory(anasys)
aux_source_directory(${PROJECT_SOURCE_DIR} VECG_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/Tools Tools_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGDemographics ECGDemographics_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGDiagnostic ECGDiagnostic_FILES)
#aux_source_directory(${PROJECT_SOURCE_DIR}/ECGGlobalMeasurements ECGGlobalMeasurements_FILES) 
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGLeadMeasurements ECGLeadMeasurements_FILES)                                            
#SET(Tools_FILES  	Tools/BytesTool.cpp )  

message("vecg_files ${VECG_FILES}")
message("Tools_FILES ${Tools_FILES}")
message("ECGDemographics_FILES ${ECGDemographics_FILES}")
message("ECGDiagnostic_FILES ${ECGDiagnostic_FILES}")
#message("ECGDiagnostic_FILES ${ECGGlobalMeasurements_FILES}")
message("ECGDiagnostic_FILES ${ECGLeadMeasurements_FILES}")
#添加头文件查找目录
include_directories(${PROJECT_SOURCE_DIR} 
					${PROJECT_SOURCE_DIR}/Tools
					${PROJECT_SOURCE_DIR}/ECGDemographics
					${PROJECT_SOURCE_DIR}/ECGDiagnostic
					${PROJECT_SOURCE_DIR}/ECGLeadMeasurements
					${PROJECT_SOURCE_DIR}/ECGSignals)
					
#设置库目录，必须放在add_library和add_executable之前
#link_directories(${PROJECT_SOURCE_DIR}/../output)

#add_definitions(-DBUILDING_DLL)

add_library(${PROJECT_NAME} ${VECG_FILES} ${Tools_FILES} ${ECGDemographics_FILES} ${ECGDiagnostic_FILES} ${ECGLeadMeasurements_FILES})

#target_link_libraries(${PROJECT_NAME}  json-c)

# add the install targets
install (TARGETS ${PROJECT_NAME}  DESTINATION ${out_dir})
#file(COPY ${PROJECT_SOURCE_DIR}/../output/json-c.dll DESTINATION ${out_dir})
#install (FILES vcg_algorithm_api.h DESTINATION ${out_dir})


