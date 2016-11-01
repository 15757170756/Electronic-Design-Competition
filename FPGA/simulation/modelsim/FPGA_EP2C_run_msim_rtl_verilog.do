transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+E:/WORKSPACE/FPGA/FPGA_EP2C {E:/WORKSPACE/FPGA/FPGA_EP2C/FPGA2MCU.v}
vlog -vlog01compat -work work +incdir+E:/WORKSPACE/FPGA/FPGA_EP2C {E:/WORKSPACE/FPGA/FPGA_EP2C/FPGA2AR9331.v}

vlog -vlog01compat -work work +incdir+E:/WORKSPACE/FPGA/FPGA_EP2C/simulation/modelsim {E:/WORKSPACE/FPGA/FPGA_EP2C/simulation/modelsim/FPGA_EP2C.vt}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneii_ver -L rtl_work -L work -voptargs="+acc"  FPGA_EP2C_vlg_tst

add wave *
view structure
view signals
run -all
