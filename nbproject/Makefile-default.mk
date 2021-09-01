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
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Principal.c graphics.c HDM64GS12.c rtcc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Principal.o ${OBJECTDIR}/graphics.o ${OBJECTDIR}/HDM64GS12.o ${OBJECTDIR}/rtcc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Principal.o.d ${OBJECTDIR}/graphics.o.d ${OBJECTDIR}/HDM64GS12.o.d ${OBJECTDIR}/rtcc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Principal.o ${OBJECTDIR}/graphics.o ${OBJECTDIR}/HDM64GS12.o ${OBJECTDIR}/rtcc.o

# Source Files
SOURCEFILES=Principal.c graphics.c HDM64GS12.c rtcc.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}



# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Principal.o: Principal.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  Principal.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/Principal.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Principal.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" Principal.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  graphics.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/graphics.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/graphics.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" graphics.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
${OBJECTDIR}/HDM64GS12.o: HDM64GS12.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  HDM64GS12.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/HDM64GS12.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/HDM64GS12.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" HDM64GS12.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
${OBJECTDIR}/rtcc.o: rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  rtcc.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/rtcc.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/rtcc.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" rtcc.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
else
${OBJECTDIR}/Principal.o: Principal.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  Principal.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/Principal.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Principal.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" Principal.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  graphics.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/graphics.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/graphics.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" graphics.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
${OBJECTDIR}/HDM64GS12.o: HDM64GS12.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  HDM64GS12.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/HDM64GS12.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/HDM64GS12.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" HDM64GS12.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
${OBJECTDIR}/rtcc.o: rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR} 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}"  rtcc.c +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 
	@mv ${OBJECTDIR}/rtcc.cof "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/rtcc.hex "dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}"" rtcc.c +EXPORT +FH +DF +CC +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P #__18F4620=1 +EXPORTD="${OBJECTDIR}"  
	
endif 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
ifeq (4,1) 
	
else 
	${MP_LD}   out="dist/${CND_CONF}/${IMAGE_TYPE}"  +FH +DF +CC LINK=SistemaEconomizador.X.${IMAGE_TYPE}.hex=${OBJECTDIR}/Principal.o,${OBJECTDIR}/graphics.o,${OBJECTDIR}/HDM64GS12.o,${OBJECTDIR}/rtcc.o +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P
	
endif 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SistemaEconomizador.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
ifeq (4,1) 
	
else 
	${MP_LD}   out="dist/${CND_CONF}/${IMAGE_TYPE}"  +FH +DF +CC LINK=SistemaEconomizador.X.${IMAGE_TYPE}.hex=${OBJECTDIR}/Principal.o,${OBJECTDIR}/graphics.o,${OBJECTDIR}/HDM64GS12.o,${OBJECTDIR}/rtcc.o +Y=9 +EA +DF +LN +T +A +M +J +EA +Z -P
	
endif 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
