file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE LV_DEV_SOURCES lv_devices/*.c)

idf_component_register(	SRCS ${SOURCES}
						SRCS ${LV_DEV_SOURCES}
						INCLUDE_DIRS . src ../ ./lv_devices
                       	PRIV_REQUIRES driver user_dev user_mid main)  