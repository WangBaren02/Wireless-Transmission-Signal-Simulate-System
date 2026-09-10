module dds_ctrl_multi
(
	input 					clk,
	input 					rst_n,

	input			[35:0]	f_word,
	input			[11:0]	p_word,
	
	output 	wire	[7:0]	dac_data
);

	wire			[7:0]	sin_data;

	reg				[35:0]	frequency_add;		//累加寄存器
	reg				[11:0]	rom_address;
	
	rom_wave_sin	rom_ip_inst_1
	(
		.address 	(rom_address),
		.clock 		(clk),
		.q 			(sin_data)
	);
	
	always@( posedge clk or negedge rst_n )
		begin
			if(!rst_n) frequency_add <= 0;
			else frequency_add <= frequency_add + f_word;
		end
		
	always@( posedge clk or negedge rst_n )
		begin
			if(!rst_n) rom_address <= 0;
			else rom_address <= frequency_add[35:24] + p_word;
		end
		
	assign dac_data = sin_data;
		
endmodule