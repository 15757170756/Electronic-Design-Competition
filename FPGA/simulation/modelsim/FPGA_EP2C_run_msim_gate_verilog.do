transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vlog -vlog01compat -work work +incdir+. {FPGA_EP2C.vo}

vlog -vlog01compat -work work +incdir+E:/WORKSPACE/FPGA/FPGA_EP2C/simulation/modelsim {E:/WORKSPACE/FPGA/FPGA_EP2C/simulation/modelsim/FPGA_EP2C.vt}

vsim -t 1ps +transport_int_delays +transport_path_delays -L cycloneii_ver -L gate_work -L work -voptargs="+acc"  FPGA_EP2C_vlg_tst

add wave *
view structure
view signals
run -all
