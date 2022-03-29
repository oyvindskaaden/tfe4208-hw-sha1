library ieee;
use ieee.std_logic_1164.all;

use work.constants.all;
use work.types.all;

entity hash_chunk_ctrl is
	port(
		-- System signals:
		signal clk :  in std_logic;
		signal rst :  in std_logic;
		
		-- Control signals:
		signal hash_chunk_start :  in std_logic;
		signal hash_chunk_done  : out std_logic;
		signal lock_words_reg   : out std_logic;
		signal lock_hash_reg    : out std_logic;
		
		-- Selector signal:
		signal sel : out w_sel
	);
end entity hash_chunk_ctrl;

architecture rtl of hash_chunk_ctrl is
	constant STARTWORD_SEL : w_sel := N_WORDS_IN_CHUNK;
	type state_t is (IDLE, MAKE_CHUNK_WORDS, SWITCH_REG, MAKE_CHUNK_HASH);
	signal state_reg, next_state : state_t := IDLE;
	signal sel_reg, next_sel : w_sel := STARTWORD_SEL;
begin	
	set_next : process (hash_chunk_start, state_reg, sel_reg) is
	begin
		next_state <= state_reg;
		
		case state_reg is
			when IDLE =>
				hash_chunk_done <= '1';
				lock_words_reg <= '1';
				lock_hash_reg <= '1';
				next_sel <= STARTWORD_SEL;
				if hash_chunk_start = '1' then
					hash_chunk_done <= '0';
					lock_words_reg <= '0';
					next_state <= MAKE_CHUNK_WORDS;
				end if;
			
			when MAKE_CHUNK_WORDS =>
				hash_chunk_done <= '0';
				lock_words_reg <= '0';
				lock_hash_reg <= '1';
				if sel_reg >= N_WORDS-1 then
					next_state <= SWITCH_REG;
					next_sel <= 0;
				else
					next_sel <= sel_reg + 1;
				end if;
			
			when SWITCH_REG =>
				hash_chunk_done <= '0';
				lock_words_reg <= '1';
				lock_hash_reg <= '0';
				next_state <= MAKE_CHUNK_HASH;
				next_sel <= 0;
			
			when MAKE_CHUNK_HASH =>
				hash_chunk_done <= '0';
				lock_words_reg <= '1';
				lock_hash_reg <= '0';
				if sel_reg >= N_WORDS-1 then
					next_state <= IDLE;
					next_sel <= sel_reg;
				else
					next_sel <= sel_reg + 1;
				end if;
		end case;
	end process set_next;
	
	sync_proc : process (clk, rst) is
	begin
		if rst = '1' then
			state_reg <= IDLE;
			sel_reg <= STARTWORD_SEL;
	
		elsif clk'event and clk = '1' then
			state_reg <= next_state;
			sel_reg <= next_sel;
		end if;
	end process sync_proc;
	
	sel <= sel_reg;
	
		
end architecture rtl;