--------------------------------------------------------------------------------
-- Company: <Name>
--
-- File: fdev_1KHz.vhd
-- Description: 
--
-- frequency devider
--
-- Targeted device: <Family::ProASIC3E> <Die::A3PE1500> <Package::208 PQFP>
-- Author: vmed
--
--------------------------------------------------------------------------------

library IEEE;


use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.numeric_std.all;

entity fdev_1KHz is
port (
    clk     : in std_logic; -- from 2MHz
    nreset  : in std_logic;
    clk_out : out std_logic -- to 1KHz
);
end fdev_1KHz;

architecture behav of fdev_1KHz is
signal N : std_logic_vector(11 downto 0);
signal pre_clk : std_logic;
begin
f_dev : process(clk, nreset)
    begin
	if nreset = '0' then
		N <= x"3E7";  -- 999
		pre_clk<='0';
	elsif rising_edge(clk) then
        N <= N + '1';
		if (N=x"3E7") then -- 999
			pre_clk<='1';
		end if;
		if (N=x"7CF") then -- 1999
			pre_clk<='0';
			N<=x"000";
		end if;
	end if;
clk_out <= pre_clk;
end process;
end behav;