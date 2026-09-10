module C_2024_2M
(
	input			sys_clk,
	input			sys_rst_n,
		
	input			spi_scl,
	input			spi_sdi,
	input			spi_sel,
	output			spi_sdo,

//	input			param_setting_done,
	output	[7:0]	DAC_data_2M_direct,
	output			DAC_clk_2M_direct,

	output	[7:0]	DAC_data_2M_multi,
	output			DAC_clk_2M_multi,

	output			DAC_9708_GND
);
/////////////////////////////////
localparam MODULATION_DEGREE = 4'd10;
////////////////////////////////
//pll
	wire	clk_120M;
	wire	clk_21M;

	wire	locked;

	pll_ip	pll_ip_inst 
	(
		.areset (~sys_rst_n),
		.inclk0 (sys_clk),
		.c0 	(clk_120M),
		.c1 	(clk_21M),
		.locked (locked)
	);

	wire	rst_n;

	assign rst_n = locked & sys_rst_n;
///////////////////////////////////////////
//	reg		param_setting_done_reg;
//
//	always@( posedge clk_120M or negedge rst_n )
//		begin
//			if(!rst_n) param_setting_done_reg <= 1'b0;
//			else param_setting_done_reg <= n param_setting_done;
//		end
//
//	wire	valid_param;
//
//	assign valid_param = ( ~ param_setting_done ) & param_setting_done_reg;
//////////////////////////////////////////////////////////////////////////////
//spi
	//SPI_STM32F4
	// wire	[7:0]       parameter_frequency_num ;
	wire	[7:0]       parameter_delay_time    ;
	// wire	[7:0]       parameter_phase         ;
	wire	[3:0]		parameter_modulation	;
//	wire	[3:0]		parameter_effective_value;
//	wire	[7:0]		parameter_70			;

	wire				spi_start				;
	wire				spi_done				;

	SPI_STM32F4 
	#(
	    .width						(6'd32)
	)
	u_SPI_STM32F4
	(
	    //system_ctrl
	    .clk         				(clk_21M),
	    .rst_n       				(rst_n),
	    //data
		.parameter_frequency_num 	(),
		.parameter_delay_time    	(parameter_delay_time),
		.parameter_phase         	(),
		 .parameter_modulation		(parameter_modulation),
		// .parameter_effective_value	(parameter_effective_value),
//		.parameter_70				(parameter_70),
	    //spi_ctrl	
	    .spi_scl     				(spi_scl),
	    .spi_sdi     				(spi_sdi),
	    .spi_sdo     				(spi_sdo),
	    .spi_sel     				(spi_sel),
		.spi_start					(spi_start),
		.spi_done					(spi_done)
	);
//////////////////////////////////////////
//	reg  	[5:0]	phase_reg;
//
//	always@( posedge clk_120M or negedge rst_n )
//		begin
//			if(!rst_n) phase_reg <= 6'b0;
//			else if(valid_param) phase_reg <= phase;
//			else phase_reg <= phase_reg;
//		end
////////////////////////////////////////
//2MHz调制信号(直达)
	DDS_lite DDS_lite_inst
	(
		.clk 	 					(clk_120M),
		.rst_n 						(rst_n),
		.En  						(spi_done),
		.clr   						(spi_start),
	
		.parameter_modulation		(parameter_modulation),
//		.parameter_effective_value	(parameter_effective_value),
//		.parameter_70				(parameter_70),
		
		.dac_data 					(DAC_data_2M_direct),
		.dac_clk 					(DAC_clk_2M_direct)
	);
/////////////////////////////////////////
	DDS_time_delay_2M DDS_time_delay_2M_inst
	(
	    .clk                    	(clk_120M),
	    .rst_n                  	(rst_n),
	
	    .DDS_En                		(spi_done),
	    .DDS_clr               		(spi_start),
	
	    .parameter_delay_time   	(parameter_delay_time),	//parameter_delay_time
	    .parameter_modulation		(parameter_modulation),
//	    .parameter_effective_value	(parameter_effective_value),
//		.parameter_70				(parameter_70),

	    .DAC_data_2M_multi      	(DAC_data_2M_multi),
		.DAC_clk_2M_multi   		(DAC_clk_2M_multi) 
	);
/////////////////////////////////////////
////2MHz调制信号(多径)
//	DDS_lite_phase DDS_lite_phase_inst
//	(
//		.clk 		(clk_120M),
//		.rst_n 		(rst_n),
//	
//		.phase 		(phase_reg),
//		
//		.dac_data 	(DAC_data_2M_multi),
//		.dac_clk 	(DAC_clk_2M_multi)
//	);
///////////////////////////////
assign DAC_9708_GND = 1'b0;
endmodule