	
#**********************************************************************
# module_digital_potentiometer (сделано на шаблонах, все .h).
#**********************************************************************
DP_H_FILE	:= $(shell find module_digital_potentiometer/ -maxdepth 3 -type f -name "*.h" )
DP_DIR		:= $(shell find module_digital_potentiometer/ -maxdepth 3 -type d -name "*" )
DP_PATH		:= $(addprefix -I, $(DP_DIR))

# Добавляем к общим переменным проекта.
PROJECT_PATH			+= $(DP_PATH)
PROJECT_OBJ_FILE		+= 