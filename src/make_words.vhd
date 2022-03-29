library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.constants.all;
use work.types.all;

entity make_words is 
	port(
		-- System signals (for words register):
		signal clk :  in std_logic;
		signal rst :  in std_logic;

		-- Selector signal:
		signal sel :  in w_sel;

		-- Data signals:
		signal chunk_in  :  in chunk;
		signal words_out : out words
	);
end entity make_words;

architecture rtl of make_words is
	signal words_in : words;
	signal mux_out0, mux_out1, mux_out2, mux_out3 : word;
	signal xor_out : word;
	signal rol_out : word;
	signal words_reg_read : words;
	signal words_to_write : unsigned(TOT_WORDS_SIZE-1 downto CHUNK_SIZE);
begin
	words_register : entity work.words_reg
		port map (
			clk      => clk,
			rst      => rst,
			words_in  => words_in,
			words_out => words_reg_read
		);
	
	mux_out0 <= words_reg_read((sel-3+1)*WORD_SIZE-1 downto (sel-3)*WORD_SIZE);
	mux_out1 <= words_reg_read((sel-8+1)*WORD_SIZE-1 downto (sel-8)*WORD_SIZE);
	mux_out2 <= words_reg_read((sel-14+1)*WORD_SIZE-1 downto (sel-14)*WORD_SIZE);
	mux_out3 <= words_reg_read((sel-16+1)*WORD_SIZE-1 downto (sel-16)*WORD_SIZE);
	
	xor_out <= mux_out0 xor mux_out1 xor mux_out2 xor mux_out3;
	rol_out <= xor_out(WORD_SIZE-2 downto 0) & xor_out(WORD_SIZE-1);
	
	words_to_write((sel+1)*WORD_SIZE-1 downto sel*WORD_SIZE) <= rol_out;

	words_in(words_to_write'range) <= words_to_write;
	words_in(chunk_in'range)       <= chunk_in;
	words_out <= words_reg_read;

end architecture rtl;