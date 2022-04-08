
module SHA1_DMA_thingymajiggy (
	clk_clk,
	nios_2_processor_custom_instruction_master_readra,
	memports1_address,
	memports1_clken,
	memports1_chipselect,
	memports1_write,
	memports1_readdata,
	memports1_writedata,
	memports1_byteenable);	

	input		clk_clk;
	output		nios_2_processor_custom_instruction_master_readra;
	input	[23:0]	memports1_address;
	input		memports1_clken;
	input		memports1_chipselect;
	input		memports1_write;
	output	[31:0]	memports1_readdata;
	input	[31:0]	memports1_writedata;
	input	[3:0]	memports1_byteenable;
endmodule
