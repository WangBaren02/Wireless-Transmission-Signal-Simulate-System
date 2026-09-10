module DDS_time_delay (
    input               clk                         ,//120M-----8.3333ns
    input               rst_n                       ,
    
    input       [5:0]   parameter_frequency_num     ,
	 input       [7:0]   parameter_delay_time        ,
    input       [7:0]   parameter_phase             ,
    input               DDS_En                      ,
    input               DDS_clr                     ,

    output              direct_dac_clk              ,
    output      [7:0]   direct_data                 ,
    output              multi_dac_clk               ,
    output      [7:0]   multi_data                  
);
    reg     [6:0]   delay_time_addr;
always @(posedge clk or negedge rst_n) begin
    if(~rst_n)
        delay_time_addr <= 7'd0;
    else case (parameter_delay_time)
        8'd50   :delay_time_addr <= parameter_frequency_num * 3'd6 - 8'd180;
        8'd80   :delay_time_addr <= parameter_frequency_num * 3'd6 - 8'd179;
        8'd110  :delay_time_addr <= parameter_frequency_num * 3'd6 - 8'd178;
        8'd140  :delay_time_addr <= parameter_frequency_num * 3'd6 - 8'd177;
        8'd170  :delay_time_addr <= parameter_frequency_num * 3'd6 - 8'd176;
        8'd200  :delay_time_addr <= parameter_frequency_num * 3'd6 - 8'd175;
        default :delay_time_addr <= 7'd70;
    endcase
end
    wire    [7:0]    delay_phase;
rom_delay_time	rom_delay_time_inst (
	.address ( delay_time_addr ),
	.clock ( clk ),
	.q ( delay_phase )
	);
/////////only to wc
    wire    [7:0]   double_phase;
assign double_phase = ((delay_phase + parameter_phase) > 8'd180) ? (9'd360 - (delay_phase + parameter_phase)) : (delay_phase + parameter_phase);
/////////
DDS_direct u_DDS_direct
(
	.clk                (clk),
	.rst_n              (rst_n),
    .En                 (DDS_En),
    .clr                (DDS_clr),

	.frequency_direct   (parameter_frequency_num),
	
	.dac_data           (direct_data),
	.dac_clk            (direct_dac_clk)
);
DDS_multi u_DDS_multi
(
	.clk                (clk),
	.rst_n              (rst_n),
    .En                 (DDS_En),
    .clr                (DDS_clr),

	.frequency_multi    (parameter_frequency_num),
	.phase              (double_phase),
	
	.dac_data           (multi_data),
	.dac_clk            (multi_dac_clk)
);
endmodule

//decimal fraction-count
//     reg     [1:0]   cnt_pluse;
// always @(posedge clk or negedge rst_n) begin
//     if(~rst_n)
//         cnt_pluse <= 2'd0;
//     else 
//         cnt_pluse <= cnt_pluse + 1'b1;
// end
//     reg     [7:0]   delay_time;
// always @(posedge clk or negedge rst_n) begin
//     if(~rst_n)
//         delay_time <= 8'd0;
//     else if(cnt_pluse == 2'd3)
//         delay_time <= delay_time + 4'd9;
//     else 
//         delay_time <= delay_time + 4'd8;
// end
// //
//     wire            DDS_multi_start;
// assign DDS_multi_start = (delay_time >= parameter_delay_time - 4'd8) ? 1'b1:1'b0;
//     reg             DDS_multi_start_keep;
// always @(posedge clk or negedge rst_n) begin
//     if(~rst_n)
//         DDS_multi_start_keep <= 1'b0;
//     else if(DDS_multi_start)
//         DDS_multi_start_keep <= 1'b1;
//     else
//         DDS_multi_start_keep <= DDS_multi_start_keep;
// end
//     wire            real_DDS_multi_start;
// assign real_DDS_multi_start = DDS_multi_start|DDS_multi_start_keep;

// //phase = 360*frequency_num*delay_time*10^-3
//     wire    [13:0]  denom;
// multiplier_unsigned	multiplier_unsigned_inst (
// 	.dataa ( parameter_delay_time ),//8bit
// 	.datab ( frequency_num ),//6bit
// 	.result ( denom )//14bit
// 	);
//     wire    [9:0]   divide_remain;
// divide1000	divide1000_inst (
// 	.denom ( 10'd1000 ),//10bit
// 	.numer ( denom ),//14bit
// 	.quotient (  ),//14bit
// 	.remain ( divide_remain )//10bit
// 	);
// //     wire    [13:0]   remain;
// // assign remain = denom >> 10;//*0.001
//     wire    [18:0]  middle_num;
// assign middle_num = divide_remain * 9'd360;
//     wire    [18:0]  phase_angle;
// assign phase_angle = middle_num >> 4'd10;
//     wire    [7:0]   real_phase_angle;
// assign real_phase_angle = (phase_angle > 8'd180) ? (19'd360 - phase_angle) : phase_angle;
