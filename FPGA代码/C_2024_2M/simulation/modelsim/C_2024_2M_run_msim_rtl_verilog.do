transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/DDS {D:/Quartus_code/C_2024_2M/rtl/DDS/DDS_time_delay_2M.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/SPI {D:/Quartus_code/C_2024_2M/rtl/SPI/SPI_STM32F4.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/SPI {D:/Quartus_code/C_2024_2M/rtl/SPI/SPI.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/ip/pll {D:/Quartus_code/C_2024_2M/ip/pll/pll_ip.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/ip/rom {D:/Quartus_code/C_2024_2M/ip/rom/rom_wave_sin.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/DDS {D:/Quartus_code/C_2024_2M/rtl/DDS/DDS_lite_phase.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/DDS {D:/Quartus_code/C_2024_2M/rtl/DDS/DDS_lite.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/DDS {D:/Quartus_code/C_2024_2M/rtl/DDS/dds_ctrl_lite_phase.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/DDS {D:/Quartus_code/C_2024_2M/rtl/DDS/dds_ctrl_lite.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/TOP {D:/Quartus_code/C_2024_2M/rtl/TOP/C_2024_2M.v}
vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/db {D:/Quartus_code/C_2024_2M/db/pll_ip_altpll.v}

vlog -vlog01compat -work work +incdir+D:/Quartus_code/C_2024_2M/rtl/DDS/tb {D:/Quartus_code/C_2024_2M/rtl/DDS/tb/tb_DDS_lite.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L fiftyfivenm_ver -L rtl_work -L work -voptargs="+acc"  tb_DDS_lite

add wave *
view structure
view signals
run -all
