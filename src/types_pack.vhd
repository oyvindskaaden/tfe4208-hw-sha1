library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.constants.all;

package types is
	-- Logic vector of one word
	subtype word is unsigned(WORD_SIZE-1 downto 0);
	
	-- Selector (index) of a word
	subtype w_sel is integer range 0 to N_WORDS-1;
	
	-- Logic vector of one chunk
	subtype chunk is unsigned(CHUNK_SIZE-1 downto 0);
	
	-- Logic vector of all words in the design
	subtype words is unsigned(TOT_WORDS_SIZE-1 downto 0);

	-- Logic vector of one hash
	subtype hash is unsigned(5*WORD_SIZE-1 downto 0);
end package types;
