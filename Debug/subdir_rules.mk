################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ili9163lcd.obj: ../ili9163lcd.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Software/TI/ccsv6/tools/compiler/msp430_15.12.2.LTS/bin/cl430" -vmsp -O3 --use_hw_mpy=none --include_path="D:/Software/TI/ccsv6/ccs_base/msp430/include" --include_path="E:/Projekte/TI_II/SPI" --include_path="D:/Software/TI/ccsv6/tools/compiler/msp430_15.12.2.LTS/include" --advice:power=all -g --define=__MSP430G2553__ --diag_warning=225 --diag_wrap=off --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="ili9163lcd.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Software/TI/ccsv6/tools/compiler/msp430_15.12.2.LTS/bin/cl430" -vmsp -O3 --use_hw_mpy=none --include_path="D:/Software/TI/ccsv6/ccs_base/msp430/include" --include_path="E:/Projekte/TI_II/SPI" --include_path="D:/Software/TI/ccsv6/tools/compiler/msp430_15.12.2.LTS/include" --advice:power=all -g --define=__MSP430G2553__ --diag_warning=225 --diag_wrap=off --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


