vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO alandtse/CommonLibVR
        REF e2a29de9ff59d998b75fbad4d7135cfef4b45ded
        SHA512 b7efae7614857d142d7f10421b8ed0a8a6657092c998a2c8a10e67c3dd766bd03e25b2ddfe53e6ae81b19382e819c54dff1e6db4c58f24102175586bcc740574
)

vcpkg_from_github(
    OUT_SOURCE_PATH OPENVR_PATH
    REPO ValveSoftware/openvr
    REF ebdea152f8aac77e9a6db29682b81d762159df7e
    SHA512 4fb668d933ac5b73eb4e97eb29816176e500a4eaebe2480cd0411c95edfb713d58312036f15db50884a2ef5f4ca44859e108dec2b982af9163cefcfc02531f63
)

# install(DIRECTORY "${OPENVR_PATH}/" DESTINATION "${SOURCE_PATH}/extern/openvr/")
file(INSTALL "${OPENVR_PATH}/headers/openvr.h" DESTINATION "${SOURCE_PATH}/extern/openvr/headers")

vcpkg_configure_cmake(
        SOURCE_PATH "${SOURCE_PATH}"
        PREFER_NINJA
        OPTIONS -DBUILD_TESTS=off -DSKSE_SUPPORT_XBYAK=on
)

vcpkg_install_cmake()
vcpkg_cmake_config_fixup(PACKAGE_NAME CommonLibSSE CONFIG_PATH lib/cmake)
vcpkg_copy_pdbs()

file(GLOB CMAKE_CONFIGS "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSE/*.cmake")
file(INSTALL ${CMAKE_CONFIGS} DESTINATION "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE")
file(INSTALL "${SOURCE_PATH}/cmake/CommonLibSSE.cmake" DESTINATION "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSE")

file(INSTALL "${OPENVR_PATH}/headers/openvr.h" DESTINATION "${CURRENT_PACKAGES_DIR}/include")

file(
        INSTALL "${SOURCE_PATH}/LICENSE"
        DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
        RENAME copyright)
