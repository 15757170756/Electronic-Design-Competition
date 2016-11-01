library verilog;
use verilog.vl_types.all;
entity BUFF is
    port(
        en              : in     vl_logic;
        write           : in     vl_logic;
        DATA_IN         : in     vl_logic_vector(15 downto 0);
        DATA_OUT        : out    vl_logic_vector(15 downto 0);
        FINISH          : out    vl_logic
    );
end BUFF;
