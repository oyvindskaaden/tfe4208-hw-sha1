library ieee;
use ieee.std_logic_1164.all;

use work.types.all;

entity word_reg is
	port(
		-- system signals:
		signal clk :  in std_logic;
		signal rst :  in std_logic;
		
		-- data signals:
		signal word_in  :  in word;
		signal word_out : out word
	);
end entity word_reg;

architecture rtl of word_reg is
begin
	process (clk, rst)
	begin
		if rst = '1' then
			word_out <= (others => '0');
		elsif clk'event and clk = '1' then
			word_out <= word_in;
		end if;
	end process;
end architecture rtl;
		