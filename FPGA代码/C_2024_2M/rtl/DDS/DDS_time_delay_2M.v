module DDS_time_delay_2M 
(
    input               clk                         ,
    input               rst_n                       ,

    input       [7:0]   parameter_delay_time        ,
    input       [3:0]   parameter_modulation        ,
//    input       [3:0]   parameter_effective_value   ,
//    input       [7:0]   parameter_70                ,

    input               DDS_En                      ,
    input               DDS_clr                     ,

    output	    [7:0]	DAC_data_2M_multi           ,
	output			    DAC_clk_2M_multi              
);
    reg     [7:0]   delay_phase;
always @(posedge clk or negedge rst_n) begin
    if(~rst_n)
        delay_phase <= 8'd0;
    else case (parameter_delay_time)
        8'd50   :delay_phase <= 8'd36;
        8'd80   :delay_phase <= 8'd58;
        8'd110  :delay_phase <= 8'd79;
        8'd140  :delay_phase <= 8'd101;
        8'd170  :delay_phase <= 8'd122;
        8'd200  :delay_phase <= 8'd144;
        default :delay_phase <= delay_phase;
    endcase
end

DDS_lite_phase u_DDS_lite_phase
(
	.clk                   (clk),
	.rst_n                 (rst_n),
    .En                    (DDS_En),
    .clr                   (DDS_clr),
    
	.phase                 (delay_phase),
    .parameter_modulation      (parameter_modulation),
//    .parameter_effective_value (parameter_effective_value),
//    .parameter_70          (parameter_70),                            
	
	.dac_data              (DAC_data_2M_multi),
	.dac_clk               (DAC_clk_2M_multi)
);





endmodule