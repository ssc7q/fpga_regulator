----------------------------------------------------------------------------------
-- File: myPWM.vhd
--
-- Description: PWM
--
-- Targeted device: <Family::ProASIC3E> <Die::A3PE1500> <Package::208 PQFP>
--
--------------------------------------------------------------------------------

library IEEE;

use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity myPWM is
port (
    input   : in std_logic_vector(31 downto 0); -- U
    clk     : in std_logic; --50Hz
    clk1    : in std_logic; --5Hz
    nreset  : in std_logic;
    Mm      : out std_logic;
    Mp      : out std_logic
    
);
end myPWM;
architecture behav of myPWM is

    constant u0         : signed(31 downto 0):=x"00024dd3"; -- 0.09

    signal preM         : std_logic_vector(4 downto 0);
begin
    call: process(clk1,nreset)
        variable u          : signed(31 downto 0);
        begin
            if nreset='1' then
                preM<=(others=>'0');

            elsif rising_edge(clk1) then
                u:=signed(input);

                if (abs(u)< u0) then                                            --             |u| < u0
                    preM(3 downto 0) <= "0000";

                elsif (abs(u)>=u0 and abs(u)<x"01800000"*u0) then               -- u0       <= |u| < 1.5*u0
                    preM(3 downto 0) <= "0010";

                elsif (abs(u)>=x"01800000"*u0 and abs(u)<x"02000000"*u0) then   -- 1.5*u0   <= |u| < 2*u0
                    preM(3 downto 0) <= "0100";

                elsif (abs(u)>=x"02000000"*u0 and abs(u)<x"02800000"*u0) then   -- 2*u0     <= |u| < 2.5*u0
                    preM(3 downto 0) <= "0110";

                elsif (abs(u)>=x"02800000"*u0 and abs(u)<x"03000000"*u0) then   -- 2.5*u0   <= |u| < 3*u0
                    preM(3 downto 0) <= "1000";

                elsif (abs(u)>=x"03000000") then                                -- 3*u0     <= |u| 
                    preM(3 downto 0) <= "1010";

                else
                    preM(3 downto 0) <= "0000";
                end if;
    
                if(u<x"00000000") then
                    preM(4) <= '1';
                else
                    preM(4) <= '0';
                end if;
            end if;
        end process;
    
    cs: process(clk,nreset)
    variable i      : integer;
    variable cnt    : unsigned(3 downto 0);
        begin
            if (nreset='1') then
                Mm  <= '0';
                Mp  <= '0';
                i   := 10;
                cnt := x"0";
            elsif rising_edge(clk) then
                if (i=10) then
                    cnt := unsigned(preM(3 downto 0));
                end if;
                
                if (cnt=x"0") then
                    Mp <= '0';
                    Mm <= '0';
                else
                    case preM(4) is
                        when '1'    =>  Mp<='1'; Mm<='0';
                        when '0'    =>  Mp<='0'; Mm<='1';
                    end case;
                    cnt:=cnt-x"1";
                end if;
                
                i := i - 1;
                if (i=0) then
                    i := 10;
                end if;
            end if;
    end process;
end behav;