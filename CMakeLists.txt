project(scp_ecg_writer)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
#add C++11 support
set (CMAKE_CXX_STANDARD 11)

#库输出目录 
SET(out_dir "${PROJECT_SOURCE_DIR}/build")


#add_subdirectory(anasys)
aux_source_directory(${PROJECT_SOURCE_DIR} Root_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/Tools Tools_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGDemographics ECGDemographics_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGDiagnostic ECGDiagnostic_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGGlobalMeasurements ECGGlobalMeasurements_FILES) 
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGLeadMeasurements ECGLeadMeasurements_FILES)       
aux_source_directory(${PROJECT_SOURCE_DIR}/ECGSignals ECGSignals_FILES)                                        
aux_source_directory(${PROJECT_SOURCE_DIR}/SCP SCP_FILES)

message("Root_FILES ${Root_FILES}")

#添加头文件查找目录
include_directories(${PROJECT_SOURCE_DIR} 
					${PROJECT_SOURCE_DIR}/Tools
					${PROJECT_SOURCE_DIR}/SCP
					${PROJECT_SOURCE_DIR}/ECGSignals
					${PROJECT_SOURCE_DIR}/ECGLeadMeasurements
					${PROJECT_SOURCE_DIR}/ECGGlobalMeasurements
					${PROJECT_SOURCE_DIR}/ECGDiagnostic
					${PROJECT_SOURCE_DIR}/ECGDemographics
					)
					
#设置库目录，必须放在add_library和add_executable之前
#link_directories(${PROJECT_SOURCE_DIR}/../output)

#add_definitions(-DBUILDING_DLL)

add_library(${PROJECT_NAME} 
			${Root_FILES} 
			${Tools_FILES} 
            ${ECGDemographics_FILES} 
            ${ECGLeadMeasurements_FILES} 
            ${ECGGlobalMeasurements_FILES}
            ${ECGDiagnostic_FILES}
			${ECGSignals_FILES}
			${SCP_FILES}
			)

#target_link_libraries(${PROJECT_NAME}  json-c)

# add the install targets
install (TARGETS ${PROJECT_NAME}  DESTINATION ${out_dir})
#file(COPY ${PROJECT_SOURCE_DIR}/../output/json-c.dll DESTINATION ${out_dir})
#install (FILES vcg_algorithm_api.h DESTINATION ${out_dir})


