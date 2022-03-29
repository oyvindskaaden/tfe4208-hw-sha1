library ieee;
use ieee.std_logic_1164.all; 

use work.constants.all;
use work.types.all;

entity words_reg is 
	port (
		-- System signals:
		clk       :  in std_logic;
		rst       :  in std_logic;
		
		-- Data signals:
		words_in  :  in words;
		words_out : out words
	);
end entity words_reg;

architecture rtl of words_reg is 
begin

	gen_word_regs : for i in N_WORDS-1 downto 0 generate
		word_reg_inst : entity work.word_reg
			port map (
				clk      => clk,
				rst      => rst,
				word_in  => words_in((i+1)*WORD_SIZE-1 downto i*WORD_SIZE),
				word_out => words_out((i+1)*WORD_SIZE-1 downto i*WORD_SIZE)
			);
	end generate gen_word_regs;

end architecture rtl;