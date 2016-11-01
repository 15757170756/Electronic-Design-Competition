library verilog;
use verilog.vl_types.all;
entity SELECT_ADDR is
    port(
        ADDR            : in     vl_logic_vector(18 downto 0);
        BUF1            : out    vl_logic
    );
end SELECT_ADDR;
