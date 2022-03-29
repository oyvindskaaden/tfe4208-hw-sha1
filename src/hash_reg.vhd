library ieee;
use ieee.std_logic_1164.all; 

use work.constants.all;
use work.types.all;

entity hash_reg is 
	port (
		-- System signals:
		clk       :  in std_logic;
		rst       :  in std_logic;
		
		-- Data signals:
		hash_in  :  in hash;
		hash_out : out hash
	);
end entity hash_reg;

architecture rtl of hash_reg is 
begin

	gen_word_regs : for i in 4 downto 0 generate
		word_reg_inst : entity work.word_reg
			port map (
				clk      => clk,
				rst      => rst,
				word_in  => hash_in((i+1)*WORD_SIZE-1 downto i*WORD_SIZE),
				word_out => hash_out((i+1)*WORD_SIZE-1 downto i*WORD_SIZE)
			);
	end generate gen_word_regs;

end architecture rtl;