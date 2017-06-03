--------------------------------------------------------------------------------
--
-- File: PWM.vhd
-- Description: 
--
-- my PWM
--
-- Targeted device: <Family::ProASIC3E> <Die::A3PE1500> <Package::208 PQFP>
-- Author: vmed
--
--------------------------------------------------------------------------------
library IEEE;

use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity PWM is
port (
    input   : in std_logic_vector(7 downto 0);
    clk     : in std_logic; --1KHz
    nreset  : in std_logic;
    enable  : in std_logic;
    M      : out std_logic
);
end PWM;
architecture behav of PWM is
begin
process(clk, nreset)
variable u : unsigned(7 downto 0);
    begin
        if (nreset='0') then
            M<='0';
        elsif (rising_edge(clk)) then
            if (enable='1') then
                u := unsigned(input(7 downto 0));
            end if;
            if(u=0) then
                M <= '0';
            else
                M <='1';
                u := u - 1;
            end if;
        end if;     
end process;
end behav;
