library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use work.constants.all;

entity completeKeyExpander is
    port(
        Z: in std_logic_vector(61 downto 0);
		keyIn: in std_logic_vector(keyLength-1 downto 0);
		keysOut: out std_logic_vector((keySegmentLength*maxCryptLoopCount-1) downto 0)
    );
end completeKeyExpander;

architecture behaviour of completeKeyExpander is
-- //////// //////// //////// //////// //////// //////// //////// ////////
	
    -- declare components
    component keyExpander
        port(
            Z: in std_logic_vector(61 downto 0);
            keyIn: in std_logic_vector((keyLength-1) downto 0);
            keyOut: out std_logic_vector((keyLength-1) downto 0);
            returnZ: out std_logic_vector(61 downto 0)
        );
    end component;
	
    -- internal latches
    type morphingZ_type is array (0 to (maxCryptLoopCount-1)) of std_logic_vector(61 downto 0);
    signal morphingZ: morphingZ_type := ( others => (others => '0') );

    type morphingKey_type is array (0 to (maxCryptLoopCount-1)) of std_logic_vector((keyLength-1) downto 0);
    signal morphingKey: morphingKey_type := ( others => (others => '0') );
	
begin
    -- connect inputs
	morphingZ(0) <= Z;
	morphingKey(0) <= keyIn;

	-- generate and connect key expanders
    keyExpansion_generation:for a in 0 to (maxCryptLoopCount-2) generate
        keyExpansion: keyExpander port map(
            Z => morphingZ(a),
            returnZ => morphingZ(a+1),
            keyIn => morphingKey(a),
            keyOut => morphingKey(a+1)
        ); 
    end generate; 
	
	-- generate output
	keysOut_generation:for a in 0 to (maxCryptLoopCount-1) generate
		keysOut((a*keySegmentLength + (keySegmentLength-1)) downto a*keySegmentLength) <= morphingKey(a)(keySegmentLength-1 downto 0);
	end generate;
 
end behaviour;