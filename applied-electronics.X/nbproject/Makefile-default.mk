#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../app/app_main.c ../app/app_menu.c ../core/board.c ../core/isr.c ../drivers/i2c_bus.c ../drivers/spi_bus.c ../drivers/uart_bt.c ../drivers/uart_pc.c ../drivers/lcd.c ../modules/bluetooth_proto.c ../modules/bmp280.c ../modules/datalogger.c ../modules/eeprom_m93c66.c ../modules/rtc_ds1307.c ../modules/sht30.c ../main.c ../app/buttons.c ../app/menu.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360919890/app_main.p1 ${OBJECTDIR}/_ext/1360919890/app_menu.p1 ${OBJECTDIR}/_ext/761097586/board.p1 ${OBJECTDIR}/_ext/761097586/isr.p1 ${OBJECTDIR}/_ext/239857660/i2c_bus.p1 ${OBJECTDIR}/_ext/239857660/spi_bus.p1 ${OBJECTDIR}/_ext/239857660/uart_bt.p1 ${OBJECTDIR}/_ext/239857660/uart_pc.p1 ${OBJECTDIR}/_ext/239857660/lcd.p1 ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1 ${OBJECTDIR}/_ext/453072328/bmp280.p1 ${OBJECTDIR}/_ext/453072328/datalogger.p1 ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1 ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1 ${OBJECTDIR}/_ext/453072328/sht30.p1 ${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/1360919890/buttons.p1 ${OBJECTDIR}/_ext/1360919890/menu.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360919890/app_main.p1.d ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d ${OBJECTDIR}/_ext/761097586/board.p1.d ${OBJECTDIR}/_ext/761097586/isr.p1.d ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d ${OBJECTDIR}/_ext/239857660/lcd.p1.d ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d ${OBJECTDIR}/_ext/453072328/bmp280.p1.d ${OBJECTDIR}/_ext/453072328/datalogger.p1.d ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d ${OBJECTDIR}/_ext/453072328/sht30.p1.d ${OBJECTDIR}/_ext/1472/main.p1.d ${OBJECTDIR}/_ext/1360919890/buttons.p1.d ${OBJECTDIR}/_ext/1360919890/menu.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360919890/app_main.p1 ${OBJECTDIR}/_ext/1360919890/app_menu.p1 ${OBJECTDIR}/_ext/761097586/board.p1 ${OBJECTDIR}/_ext/761097586/isr.p1 ${OBJECTDIR}/_ext/239857660/i2c_bus.p1 ${OBJECTDIR}/_ext/239857660/spi_bus.p1 ${OBJECTDIR}/_ext/239857660/uart_bt.p1 ${OBJECTDIR}/_ext/239857660/uart_pc.p1 ${OBJECTDIR}/_ext/239857660/lcd.p1 ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1 ${OBJECTDIR}/_ext/453072328/bmp280.p1 ${OBJECTDIR}/_ext/453072328/datalogger.p1 ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1 ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1 ${OBJECTDIR}/_ext/453072328/sht30.p1 ${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/1360919890/buttons.p1 ${OBJECTDIR}/_ext/1360919890/menu.p1

# Source Files
SOURCEFILES=../app/app_main.c ../app/app_menu.c ../core/board.c ../core/isr.c ../drivers/i2c_bus.c ../drivers/spi_bus.c ../drivers/uart_bt.c ../drivers/uart_pc.c ../drivers/lcd.c ../modules/bluetooth_proto.c ../modules/bmp280.c ../modules/datalogger.c ../modules/eeprom_m93c66.c ../modules/rtc_ds1307.c ../modules/sht30.c ../main.c ../app/buttons.c ../app/menu.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26K83
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360919890/app_main.p1: ../app/app_main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/app_main.p1 ../app/app_main.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/app_main.d ${OBJECTDIR}/_ext/1360919890/app_main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/app_main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360919890/app_menu.p1: ../app/app_menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/app_menu.p1 ../app/app_menu.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/app_menu.d ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/761097586/board.p1: ../core/board.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/761097586" 
	@${RM} ${OBJECTDIR}/_ext/761097586/board.p1.d 
	@${RM} ${OBJECTDIR}/_ext/761097586/board.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/761097586/board.p1 ../core/board.c 
	@-${MV} ${OBJECTDIR}/_ext/761097586/board.d ${OBJECTDIR}/_ext/761097586/board.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/761097586/board.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/761097586/isr.p1: ../core/isr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/761097586" 
	@${RM} ${OBJECTDIR}/_ext/761097586/isr.p1.d 
	@${RM} ${OBJECTDIR}/_ext/761097586/isr.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/761097586/isr.p1 ../core/isr.c 
	@-${MV} ${OBJECTDIR}/_ext/761097586/isr.d ${OBJECTDIR}/_ext/761097586/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/761097586/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/i2c_bus.p1: ../drivers/i2c_bus.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/i2c_bus.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/i2c_bus.p1 ../drivers/i2c_bus.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/i2c_bus.d ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/spi_bus.p1: ../drivers/spi_bus.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/spi_bus.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/spi_bus.p1 ../drivers/spi_bus.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/spi_bus.d ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/uart_bt.p1: ../drivers/uart_bt.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_bt.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/uart_bt.p1 ../drivers/uart_bt.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/uart_bt.d ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/uart_pc.p1: ../drivers/uart_pc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_pc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/uart_pc.p1 ../drivers/uart_pc.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/uart_pc.d ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/lcd.p1: ../drivers/lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/lcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/lcd.p1 ../drivers/lcd.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/lcd.d ${OBJECTDIR}/_ext/239857660/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1: ../modules/bluetooth_proto.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1 ../modules/bluetooth_proto.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.d ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/bmp280.p1: ../modules/bmp280.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/bmp280.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/bmp280.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/bmp280.p1 ../modules/bmp280.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/bmp280.d ${OBJECTDIR}/_ext/453072328/bmp280.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/bmp280.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/datalogger.p1: ../modules/datalogger.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/datalogger.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/datalogger.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/datalogger.p1 ../modules/datalogger.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/datalogger.d ${OBJECTDIR}/_ext/453072328/datalogger.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/datalogger.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1: ../modules/eeprom_m93c66.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1 ../modules/eeprom_m93c66.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.d ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1: ../modules/rtc_ds1307.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1 ../modules/rtc_ds1307.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.d ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/sht30.p1: ../modules/sht30.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/sht30.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/sht30.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/sht30.p1 ../modules/sht30.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/sht30.d ${OBJECTDIR}/_ext/453072328/sht30.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/sht30.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/main.p1 ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360919890/buttons.p1: ../app/buttons.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/buttons.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/buttons.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/buttons.p1 ../app/buttons.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/buttons.d ${OBJECTDIR}/_ext/1360919890/buttons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/buttons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360919890/menu.p1: ../app/menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/menu.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/menu.p1 ../app/menu.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/menu.d ${OBJECTDIR}/_ext/1360919890/menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1360919890/app_main.p1: ../app/app_main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/app_main.p1 ../app/app_main.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/app_main.d ${OBJECTDIR}/_ext/1360919890/app_main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/app_main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360919890/app_menu.p1: ../app/app_menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/app_menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/app_menu.p1 ../app/app_menu.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/app_menu.d ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/app_menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/761097586/board.p1: ../core/board.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/761097586" 
	@${RM} ${OBJECTDIR}/_ext/761097586/board.p1.d 
	@${RM} ${OBJECTDIR}/_ext/761097586/board.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/761097586/board.p1 ../core/board.c 
	@-${MV} ${OBJECTDIR}/_ext/761097586/board.d ${OBJECTDIR}/_ext/761097586/board.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/761097586/board.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/761097586/isr.p1: ../core/isr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/761097586" 
	@${RM} ${OBJECTDIR}/_ext/761097586/isr.p1.d 
	@${RM} ${OBJECTDIR}/_ext/761097586/isr.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/761097586/isr.p1 ../core/isr.c 
	@-${MV} ${OBJECTDIR}/_ext/761097586/isr.d ${OBJECTDIR}/_ext/761097586/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/761097586/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/i2c_bus.p1: ../drivers/i2c_bus.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/i2c_bus.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/i2c_bus.p1 ../drivers/i2c_bus.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/i2c_bus.d ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/i2c_bus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/spi_bus.p1: ../drivers/spi_bus.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/spi_bus.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/spi_bus.p1 ../drivers/spi_bus.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/spi_bus.d ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/spi_bus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/uart_bt.p1: ../drivers/uart_bt.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_bt.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/uart_bt.p1 ../drivers/uart_bt.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/uart_bt.d ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/uart_bt.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/uart_pc.p1: ../drivers/uart_pc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/uart_pc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/uart_pc.p1 ../drivers/uart_pc.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/uart_pc.d ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/uart_pc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/239857660/lcd.p1: ../drivers/lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/239857660" 
	@${RM} ${OBJECTDIR}/_ext/239857660/lcd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/239857660/lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/239857660/lcd.p1 ../drivers/lcd.c 
	@-${MV} ${OBJECTDIR}/_ext/239857660/lcd.d ${OBJECTDIR}/_ext/239857660/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/239857660/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1: ../modules/bluetooth_proto.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1 ../modules/bluetooth_proto.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.d ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/bluetooth_proto.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/bmp280.p1: ../modules/bmp280.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/bmp280.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/bmp280.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/bmp280.p1 ../modules/bmp280.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/bmp280.d ${OBJECTDIR}/_ext/453072328/bmp280.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/bmp280.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/datalogger.p1: ../modules/datalogger.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/datalogger.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/datalogger.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/datalogger.p1 ../modules/datalogger.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/datalogger.d ${OBJECTDIR}/_ext/453072328/datalogger.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/datalogger.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1: ../modules/eeprom_m93c66.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1 ../modules/eeprom_m93c66.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.d ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/eeprom_m93c66.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1: ../modules/rtc_ds1307.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1 ../modules/rtc_ds1307.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.d ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/rtc_ds1307.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/453072328/sht30.p1: ../modules/sht30.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/453072328" 
	@${RM} ${OBJECTDIR}/_ext/453072328/sht30.p1.d 
	@${RM} ${OBJECTDIR}/_ext/453072328/sht30.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/453072328/sht30.p1 ../modules/sht30.c 
	@-${MV} ${OBJECTDIR}/_ext/453072328/sht30.d ${OBJECTDIR}/_ext/453072328/sht30.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/453072328/sht30.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/main.p1 ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360919890/buttons.p1: ../app/buttons.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/buttons.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/buttons.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/buttons.p1 ../app/buttons.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/buttons.d ${OBJECTDIR}/_ext/1360919890/buttons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/buttons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360919890/menu.p1: ../app/menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1360919890" 
	@${RM} ${OBJECTDIR}/_ext/1360919890/menu.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1360919890/menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1360919890/menu.p1 ../app/menu.c 
	@-${MV} ${OBJECTDIR}/_ext/1360919890/menu.d ${OBJECTDIR}/_ext/1360919890/menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1360919890/menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../app/app_main.c
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=pickit3  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../app/app_main.c
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/applied-electronics.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
