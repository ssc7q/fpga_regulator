--------------------------------------------------------------------------------
-- Company: -
--
-- File: myRevCount.vhd

-- Description: 
--
-- Count for positive and negative impulses from Angular rate sensor
--
-- Targeted device: <Family::ProASIC3E> <Die::A3PE1500> <Package::208 PQFP>
-- Author: vmed
--
--------------------------------------------------------------------------------

library IEEE;

use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

entity myRevCount is
port (
    clk : in std_logic; --clk GLB 2MHz
    nEnable : in std_logic;
    pEnable : in std_logic;
    nreset : in std_logic;
    count : out std_logic_vector(10 downto 0)
);
end myRevCount;
architecture behav of myRevCount is
    signal pre_count: std_logic_vector(10 downto 0);
begin process (clk, nEnable, pEnable, nreset)
begin  
    if nreset= '0' then
        pre_count <= "00000000000";
    elsif (rising_edge(clk)) then
        if (pEnable='1' and nEnable='0') then
            pre_count <= pre_count + '1';
        elsif (pEnable='0' and nEnable='1') then
            pre_count <= pre_count - '1';
        end if;
    end if;
end process;
count <= pre_count;
end behav;
