#---- This script build and bundle iOS build lib of simulator/device 
# into one fat Universal lib. 

MY_PRODUCT="IcEngLib_iOS"
DIR_BUNDLE="${BUILD_DIR}/lib/${CONFIGURATION}"
FILE_BUNDLE="${DIR_BUNDLE}/lib${MY_PRODUCT}.a"
FILE_LIB_DEV="${BUILD_DIR}/tmp/${CONFIGURATION}-iphoneos/lib${MY_PRODUCT}.a"
FILE_LIB_SIM="${BUILD_DIR}/tmp/${CONFIGURATION}-iphonesimulator/lib${MY_PRODUCT}.a"

xcodebuild -target ${MY_PRODUCT} ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphoneos  
xcodebuild -target ${MY_PRODUCT} ONLY_ACTIVE_ARCH=NO -configuration ${CONFIGURATION} -sdk iphonesimulator

mkdir -p "${DIR_BUNDLE}"
lipo -create -output "${FILE_BUNDLE}" "${FILE_LIB_DEV}" "${FILE_LIB_SIM}"
