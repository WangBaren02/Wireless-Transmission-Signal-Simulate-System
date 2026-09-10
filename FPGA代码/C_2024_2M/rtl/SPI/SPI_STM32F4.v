module SPI_STM32F4 
#(
    parameter                       width = 6'd32
)
(
    //system_ctrl
    input                           clk                     ,
    input                           rst_n                   ,
    //data
    output              [7:0]       parameter_frequency_num ,
    output              [7:0]       parameter_delay_time    ,
    output              [7:0]       parameter_phase         ,
    output              [3:0]       parameter_modulation    ,
//    output              [7:0]       parameter_70            ,
    //spi_ctrl
    input   wire                    spi_scl                 ,
    input   wire                    spi_sdi                 ,
    output  wire                    spi_sdo                 ,
    input   wire                    spi_sel                 ,
    output                          spi_start               ,
    output                          spi_done                
);
//spi_ctrl
    wire    [width - 1:0]           Dout/*synthesis keep*/;
SPI 
#(
    .width                      (width)
)
u_SPI_inst
(
    .clk                        (clk)   ,
    .rst_n                      (rst_n) ,

    .spi_scl                    (spi_scl),
    .spi_sdi                    (spi_sdi),
    .spi_sdo                    (spi_sdo),
    .spi_sel                    (spi_sel),

    .Din                        (),
    .Dout                       (Dout),
    .Data_begin                 (spi_start),
    .Data_end                   (spi_done)
);
// assign spi_done                 = Dout[31];//8bit-0x80
assign parameter_modulation     = Dout[27:24];
//assign parameter_70             = Dout[31:24];
assign parameter_frequency_num  = Dout[23:16];//8bit
assign parameter_delay_time     = Dout[15:8];//8bit
assign parameter_phase          = Dout[7:0];//8bit

endmodule
