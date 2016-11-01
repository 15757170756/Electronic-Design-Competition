library verilog;
use verilog.vl_types.all;
entity BUFF_SEND_DATA is
    port(
        DATA_IN         : in     vl_logic_vector(15 downto 0);
        READ            : in     vl_logic;
        DATA_OUT        : out    vl_logic_vector(15 downto 0);
        FINISH          : out    vl_logic
    );
end BUFF_SEND_DATA;
