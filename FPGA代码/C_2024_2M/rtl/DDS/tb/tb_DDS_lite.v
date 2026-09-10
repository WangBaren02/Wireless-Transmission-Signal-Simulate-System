`timescale 1ns/1ns

module tb_DDS_lite();

reg	clk;
reg	rst_n;
reg	En;
reg clr;

reg	[3:0]	parameter_modulation;

wire	[7:0]	dac_data;
wire			dac_clk;

DDS_lite DDS_lite_inst
(
	.clk 			 		(clk),
	.rst_n 					(rst_n),
	.En 					(En),
	.clr 					(clr),

	.parameter_modulation 	(parameter_modulation),
	
	.dac_data 				(dac_data),
	.dac_clk				(dac_clk)
);

initial
	begin
		clk = 1'b1;
		rst_n <= 1'b0;
		En <= 1'b1;
		clr <= 1'b0;

		parameter_modulation <= 4'd6;

		#1000
		rst_n <= 1'b1;
	end

	always #10 clk = ~clk;

endmodule