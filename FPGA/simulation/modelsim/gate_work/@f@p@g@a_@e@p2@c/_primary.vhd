library verilog;
use verilog.vl_types.all;
entity FPGA_EP2C is
    port(
        INT0            : out    vl_logic;
        INT4            : out    vl_logic;
        AD_IN           : inout  vl_logic_vector(15 downto 0);
        NOE             : in     vl_logic;
        NADV            : in     vl_logic;
        A16             : in     vl_logic;
        A17             : in     vl_logic;
        A18             : in     vl_logic;
        NWE             : in     vl_logic
    );
end FPGA_EP2C;
