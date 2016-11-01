onerror {quit -f}
vlib work
vlog -work work FPGA_EP2C.vo
vlog -work work FPGA_EP2C.vt
vsim -novopt -c -t 1ps -L cycloneive_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate work.FPGA_EP2C_vlg_vec_tst
vcd file -direction FPGA_EP2C.msim.vcd
vcd add -internal FPGA_EP2C_vlg_vec_tst/*
vcd add -internal FPGA_EP2C_vlg_vec_tst/i1/*
add wave /*
run -all
