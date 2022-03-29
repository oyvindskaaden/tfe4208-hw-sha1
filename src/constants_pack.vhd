library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

package constants is
	
	-- Number of bits in a word
	-- NB: reducing WORD_SIZE below 
	constant WORD_SIZE    : integer := 32;
	
	-- Number of words and bits in a chunk
	-- NB: reducing N_WORDS_IN_CHUNK will break make_words
	constant N_WORDS_IN_CHUNK : integer := 16;
	constant CHUNK_SIZE       : integer := N_WORDS_IN_CHUNK*WORD_SIZE;

	-- Number of words
	constant N_WORDS      : integer := 80;
	constant N_WORDS_LOG2 : integer := integer(ceil(log2(real(N_WORDS))));
	
	-- Number of bits in all words in total
	constant TOT_WORDS_SIZE : integer := N_WORDS*WORD_SIZE;

	

end package constants;