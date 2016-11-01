library verilog;
use verilog.vl_types.all;
entity SAVE_ADDR is
    port(
        NADV            : in     vl_logic;
        AD_IN           : in     vl_logic_vector(15 downto 0);
        A16             : in     vl_logic;
        A17             : in     vl_logic;
        A18             : in     vl_logic;
        ADDR            : out    vl_logic_vector(18 downto 0)
    );
end SAVE_ADDR;
