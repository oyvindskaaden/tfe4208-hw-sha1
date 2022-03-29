library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.constants.all;
use work.types.all;

entity hash_chunk is
	port(
		-- System signals (for hash register):
		signal clk :  in std_logic;
		signal rst :  in std_logic;
	
		-- Selector signal:
		signal sel :  in w_sel;
		
		-- Data signals:
		signal words_out :  in words;
		signal hash_out  :  in hash;
		signal chunk_hash_out  : out hash
	);
end entity hash_chunk;

architecture rtl of hash_chunk is
	signal chunk_hash_in : hash := hash_out;
	signal chunk_hash_reg_read : hash;
	signal a_reg, next_a : word;
	signal b_reg, next_b : word;
	signal c_reg, next_c : word;
	signal d_reg, next_d : word;
	signal e_reg, next_e : word;
begin
	chunk_hash_register : entity work.hash_reg
		port map (
			clk      => clk,
			rst      => rst,
			hash_in  => chunk_hash_in,
			hash_out => chunk_hash_reg_read
		);


	hash_chunk_proc : process (sel, words_out, a_reg, b_reg, c_reg, d_reg, e_reg) is
--		variable i : integer;
		variable f, k, a_rol : word;
	begin
--		i := vec_to_int(sel);
		if (0 <= sel) and (sel <= 19) then
			f := (b_reg and c_reg) or ((not b_reg) and d_reg);
			k := to_unsigned(16#5A827999#, k'length);

		elsif (20 <= sel) and (sel <= 39) then
			f := b_reg xor c_reg xor d_reg;
			k := to_unsigned(16#6ED9EBA1#, k'length);

		elsif (40 <= sel) and (sel <= 59) then
			f := (b_reg and c_reg) or (b_reg and d_reg) or (c_reg and d_reg);
			k := to_unsigned(16#8F1BBCDC#, k'length);

		elsif (60 <= sel) and (sel <= 79) then
			f := b_reg xor c_reg xor d_reg;
			k := to_unsigned(16#CA62C1D6#, k'length);
		end if;

		a_rol := a_reg(WORD_SIZE-6 downto 0) & a_reg(WORD_SIZE-1 downto WORD_SIZE-5);
		next_a <= a_rol + f + e_reg + k + words_out((sel+1)*WORD_SIZE-1 downto sel*WORD_SIZE);
		next_b <= a_reg;
		next_c <= b_reg(WORD_SIZE-31 downto 0) & b_reg(WORD_SIZE-1 downto WORD_SIZE-30);
		next_d <= c_reg;
		next_e <= d_reg;
		
	end process hash_chunk_proc;
	
	a_reg <= chunk_hash_reg_read(5*WORD_SIZE-1 downto 4*WORD_SIZE);
	b_reg <= chunk_hash_reg_read(4*WORD_SIZE-1 downto 3*WORD_SIZE);
	c_reg <= chunk_hash_reg_read(3*WORD_SIZE-1 downto 2*WORD_SIZE);
	d_reg <= chunk_hash_reg_read(2*WORD_SIZE-1 downto 1*WORD_SIZE);
	e_reg <= chunk_hash_reg_read(1*WORD_SIZE-1 downto 0*WORD_SIZE);
	
	chunk_hash_in(5*WORD_SIZE-1 downto 4*WORD_SIZE) <= next_a;
	chunk_hash_in(4*WORD_SIZE-1 downto 3*WORD_SIZE) <= next_b;
	chunk_hash_in(3*WORD_SIZE-1 downto 2*WORD_SIZE) <= next_c;
	chunk_hash_in(2*WORD_SIZE-1 downto 1*WORD_SIZE) <= next_d;
	chunk_hash_in(1*WORD_SIZE-1 downto 0*WORD_SIZE) <= next_e;
	
	chunk_hash_out <= chunk_hash_reg_read;

end architecture rtl;