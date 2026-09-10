module DDS_direct
(
	input				clk,
	input				rst_n,
	input				En,
	input				clr,

	input	[5:0]		frequency_direct,
	
	output	[7:0]		dac_data,
	output				dac_clk
);
///////////////////////////////////////////////////
	reg		[35:0]		f_word;

	always@( posedge clk or negedge rst_n )
		begin
			if(!rst_n) f_word <= 36'd0;
			else 
				case(frequency_direct)
				6'd30:	f_word <= 36'd17179869184;
				6'd31:	f_word <= 36'd17752531490;
				6'd32:	f_word <= 36'd18325193796;
				6'd33:	f_word <= 36'd18897856102;
				6'd34:	f_word <= 36'd19470518408;
				6'd35:	f_word <= 36'd20043180714;
				6'd36:	f_word <= 36'd20615843020;
				6'd37:	f_word <= 36'd21188505327;
				6'd38:	f_word <= 36'd21761167633;
				6'd39:	f_word <= 36'd22333829939;
				6'd40:	f_word <= 36'd22906492245;
				default:f_word <= f_word;
				endcase
		end
////////////////////////////////////////////////////
	reg		[3:0]   En_r;

	always@( posedge clk or negedge rst_n ) 
		begin
			if(~rst_n) En_r <= 3'd0;
			else En_r <= {En_r[2],En_r[1],En_r[0],En};
		end
/////////////////////////////////////////////////////////
	reg				DDS_en;

	always@( posedge clk or negedge rst_n or posedge clr ) 
		begin
			if((~rst_n)||(clr)) DDS_en <= 1'b0;
			else if(En_r[3]) DDS_en <= 1'b1;
			else DDS_en <= DDS_en;
		end
///////////////////////////////////////////
	dds_ctrl_direct	dds_ctrl_direct_inst
	(
		.clk		(clk),
		.rst_n		(DDS_en),
		.f_word		(f_word),
	
		.dac_data	(dac_data)
	);
	
	assign dac_clk = ~ clk;
	
endmodule