quietly set ACTELLIBNAME ProASIC3E
quietly set PROJECT_DIR "C:/Microsemi/Libero_v11.5/projects/testMyCount"

if {[file exists presynth/_info]} {
   echo "INFO: Simulation library presynth already exists"
} else {
   file delete -force presynth 
   vlib presynth
}
vmap presynth presynth
vmap proasic3e "C:/Microsemi/Libero_v11.5/Designer/lib/modelsim/precompiled/vhdl/proasic3e"

vcom -93 -explicit  -work presynth "${PROJECT_DIR}/hdl/myCount.vhd"
vcom -93 -explicit  -work presynth "${PROJECT_DIR}/component/work/myCount/myCount.vhd"
vcom -93 -explicit  -work presynth "${PROJECT_DIR}/stimulus/testbench.vhd"

vsim -L ProASIC3E -L presynth  -t 1ps presynth.testbench
do "wave.do"
run 100000ns
