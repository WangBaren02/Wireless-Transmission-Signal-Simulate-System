`timescale 1ns/1ns

module tb_test_DDS();

reg	sys_clk;
reg	sys_rst_n;

wire	[7:0]	DAC_data_direct;
wire			DAC_clk_direct;

wire			AD9708_GND;

test_DDS test_DDS_inst
(
	.sys_clk 			(sys_clk),
	.sys_rst_n 			(sys_rst_n),

//	.param_setting_done	(),

//	.select				(),

//	.DAC_data_2M 		(),
//	.DAC_clk_2M			(),

	.DAC_data_direct 	(DAC_data_direct),
	.DAC_clk_direct		(DAC_clk_direct),

//	.DAC_data_multi		(),
//	.DAC_clk_multi		(),

	.AD9708_GND			(AD9708_GND)
);

initial
	begin
		sys_clk = 1'b1;
		sys_rst_n <= 1'b0;

		#1000
		sys_rst_n <= 1'b1;
	end

always #1 sys_clk = ~ sys_clk;

endmodule