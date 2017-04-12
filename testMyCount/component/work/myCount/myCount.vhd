----------------------------------------------------------------------
-- Created by SmartDesign Wed Apr 12 11:19:07 2017
-- Version: v11.5 SP2 11.5.2.6
----------------------------------------------------------------------

----------------------------------------------------------------------
-- Libraries
----------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

library proasic3e;
use proasic3e.all;
----------------------------------------------------------------------
-- myCount entity declaration
----------------------------------------------------------------------
entity myCount is
    -- Port list
    port(
        -- Inputs
        clk    : in  std_logic;
        nIMP   : in  std_logic;
        nreset : in  std_logic;
        pIMP   : in  std_logic;
        -- Outputs
        count  : out std_logic_vector(10 downto 0)
        );
end myCount;
----------------------------------------------------------------------
-- myCount architecture body
----------------------------------------------------------------------
architecture RTL of myCount is
----------------------------------------------------------------------
-- Component declarations
----------------------------------------------------------------------
-- AND2
component AND2
    -- Port list
    port(
        -- Inputs
        A : in  std_logic;
        B : in  std_logic;
        -- Outputs
        Y : out std_logic
        );
end component;
-- AND2A
component AND2A
    -- Port list
    port(
        -- Inputs
        A : in  std_logic;
        B : in  std_logic;
        -- Outputs
        Y : out std_logic
        );
end component;
-- DFN1
component DFN1
    -- Port list
    port(
        -- Inputs
        CLK : in  std_logic;
        D   : in  std_logic;
        -- Outputs
        Q   : out std_logic
        );
end component;
-- myRevCount
component myRevCount
    -- Port list
    port(
        -- Inputs
        clk     : in  std_logic;
        nEnable : in  std_logic;
        nreset  : in  std_logic;
        pEnable : in  std_logic;
        -- Outputs
        count   : out std_logic_vector(10 downto 0)
        );
end component;
----------------------------------------------------------------------
-- Signal declarations
----------------------------------------------------------------------
signal AND2_0_Y    : std_logic;
signal AND2_1_Y    : std_logic;
signal AND2A_0_Y   : std_logic;
signal AND2A_1_Y   : std_logic;
signal count_net_0 : std_logic_vector(10 downto 0);
signal DFN1_0_Q    : std_logic;
signal DFN1_1_Q    : std_logic;
signal DFN1_2_Q    : std_logic;
signal DFN1_3_Q    : std_logic;
signal DFN1_4_Q    : std_logic;
signal DFN1_5_Q    : std_logic;
signal count_net_1 : std_logic_vector(10 downto 0);

begin
----------------------------------------------------------------------
-- Top level output port assignments
----------------------------------------------------------------------
 count_net_1        <= count_net_0;
 count(10 downto 0) <= count_net_1;
----------------------------------------------------------------------
-- Component instances
----------------------------------------------------------------------
-- AND2_0
AND2_0 : AND2
    port map( 
        -- Inputs
        A => DFN1_0_Q,
        B => DFN1_2_Q,
        -- Outputs
        Y => AND2_0_Y 
        );
-- AND2_1
AND2_1 : AND2
    port map( 
        -- Inputs
        A => DFN1_3_Q,
        B => DFN1_4_Q,
        -- Outputs
        Y => AND2_1_Y 
        );
-- AND2A_0
AND2A_0 : AND2A
    port map( 
        -- Inputs
        A => DFN1_1_Q,
        B => AND2_0_Y,
        -- Outputs
        Y => AND2A_0_Y 
        );
-- AND2A_1
AND2A_1 : AND2A
    port map( 
        -- Inputs
        A => DFN1_5_Q,
        B => AND2_1_Y,
        -- Outputs
        Y => AND2A_1_Y 
        );
-- DFN1_0
DFN1_0 : DFN1
    port map( 
        -- Inputs
        D   => pIMP,
        CLK => clk,
        -- Outputs
        Q   => DFN1_0_Q 
        );
-- DFN1_1
DFN1_1 : DFN1
    port map( 
        -- Inputs
        D   => DFN1_2_Q,
        CLK => clk,
        -- Outputs
        Q   => DFN1_1_Q 
        );
-- DFN1_2
DFN1_2 : DFN1
    port map( 
        -- Inputs
        D   => DFN1_0_Q,
        CLK => clk,
        -- Outputs
        Q   => DFN1_2_Q 
        );
-- DFN1_3
DFN1_3 : DFN1
    port map( 
        -- Inputs
        D   => nIMP,
        CLK => clk,
        -- Outputs
        Q   => DFN1_3_Q 
        );
-- DFN1_4
DFN1_4 : DFN1
    port map( 
        -- Inputs
        D   => DFN1_3_Q,
        CLK => clk,
        -- Outputs
        Q   => DFN1_4_Q 
        );
-- DFN1_5
DFN1_5 : DFN1
    port map( 
        -- Inputs
        D   => DFN1_4_Q,
        CLK => clk,
        -- Outputs
        Q   => DFN1_5_Q 
        );
-- myRevCount_0
myRevCount_0 : myRevCount
    port map( 
        -- Inputs
        clk     => clk,
        nEnable => AND2A_1_Y,
        pEnable => AND2A_0_Y,
        nreset  => nreset,
        -- Outputs
        count   => count_net_0 
        );

end RTL;
