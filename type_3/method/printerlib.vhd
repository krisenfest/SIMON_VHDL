-- note: library's need to be "-a" 'd before all others

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use std.textio.all; 

package printerlib is
    shared variable consoleBuffer: line;
    function toString(inputVal: std_logic) return string;
    function toString(inputVal: std_logic_vector) return string;
    procedure print(inputVal: string);
    procedure flush;
    function toHexString(inputVal: std_logic_vector) return string;
    procedure logicPrint(inputVal: std_logic);
    procedure logicPrint(inputVal: std_logic_vector);
end printerlib;

package body printerlib is
    function toString(inputVal: std_logic) 
    return string is variable returnedString: string(3 downto 1);
    begin returnedString := std_logic'image(inputVal); return "" & returnedString(2);
    end function;
    function toString(inputVal: std_logic_vector)
    return string is variable returnedString: string(inputVal'length downto 1);
    begin for a in 1 to returnedString'length loop returnedString(a) := toString(inputVal(a-1))(1); end loop; return returnedString;
    end function;
    procedure print(inputVal: string) is begin write(consoleBuffer,inputVal); end procedure;
    procedure flush is begin writeline(output,consoleBuffer); end procedure;
    function toHexString(inputVal: std_logic_vector)
    return string is 
        variable returnedString: string((inputVal'length)/4 downto 0);
        variable a: integer := 0; variable counter: integer := (inputVal'length)/4;
        variable workingVector: std_logic_vector(3 downto 0) := "0000";
    begin
        if( ((inputVal'length) mod 4) /= 0 ) then return "binary: " & toString(inputVal); end if;
        while(a < inputVal'length) loop
            workingVector := inputVal((inputVal'length)-1-a downto (inputVal'length)-4-a);
            case workingVector is
                when "0000" => returnedString(counter) := '0';
                when "0001" => returnedString(counter) := '1';  
                when "0010" => returnedString(counter) := '2';
                when "0011" => returnedString(counter) := '3';  
                when "0100" => returnedString(counter) := '4';
                when "0101" => returnedString(counter) := '5';  
                when "0110" => returnedString(counter) := '6';
                when "0111" => returnedString(counter) := '7';  
                when "1000" => returnedString(counter) := '8';
                when "1001" => returnedString(counter) := '9';  
                when "1010" => returnedString(counter) := 'a';
                when "1011" => returnedString(counter) := 'b';  
                when "1100" => returnedString(counter) := 'c';
                when "1101" => returnedString(counter) := 'd';  
                when "1110" => returnedString(counter) := 'e';
                when "1111" => returnedString(counter) := 'f';  
                when others =>
            end case;
            counter := counter - 1;
            a := a + 4;
        end loop;
    return returnedString;
    end function;
    procedure logicPrint(inputVal: std_logic) is variable l: line;
    begin print(toString(inputVal)); end procedure;
    procedure logicPrint(inputVal: std_logic_vector) is variable l: line;
    begin print(toHexString(inputVal)); end procedure;
end printerlib;