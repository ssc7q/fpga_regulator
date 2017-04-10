--------------------------------------------------------------------------------
-- Company: <Name>
--
-- File: testbench.vhd
-- File history:
--      <Revision number>: <Date>: <Comments>
--      <Revision number>: <Date>: <Comments>
--      <Revision number>: <Date>: <Comments>
--
-- Description: 
--
-- <Description here>
--
-- Targeted device: <Family::ProASIC3E> <Die::A3PE1500> <Package::208 PQFP>
-- Author: <Name>
--
--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;

entity testbench is
end testbench;

architecture behavioral of testbench is

    constant SYSCLK_PERIOD : time := 500 ns; -- 2MHZ

    signal SYSCLK : std_logic := '0';
    signal NSYSRESET : std_logic := '0';
    signal NIMPULSE : std_logic := '0';
    signal PIMPULSE : std_logic := '0';

    component myCount
        -- ports
        port( 
            -- Inputs
            clk : in std_logic;
            pIMP : in std_logic;
            nIMP : in std_logic;
            nreset : in std_logic;

            -- Outputs
            count : out std_logic_vector(10 downto 0)

            -- Inouts

        );
    end component;

begin

    process
        variable vhdl_initial : BOOLEAN := TRUE;

    begin
        if ( vhdl_initial ) then
            -- Assert Reset
            NSYSRESET <= '0';
            wait for ( SYSCLK_PERIOD * 10 );
            
            NSYSRESET <= '1';
            wait;
        end if;
    end process;

    -- Clock Driver
    SYSCLK <= not SYSCLK after (SYSCLK_PERIOD / 2.0 );
    PIMPULSE <= not PIMPULSE after (SYSCLK_PERIOD * 15);
    NIMPULSE <= not NIMPULSE after (SYSCLK_PERIOD * 10);
    -- Instantiate Unit Under Test:  myCount
    myCount_0 : myCount
        -- port map
        port map( 
            -- Inputs
            clk => SYSCLK,
            pIMP => PIMPULSE,
            nIMP => NIMPULSE,
            nreset => NSYSRESET,

            -- Outputs
            count => open

            -- Inouts

        );

end behavioral;

