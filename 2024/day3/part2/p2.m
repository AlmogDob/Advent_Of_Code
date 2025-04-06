clc; clear; close all;

temp = readlines('test_input.txt')
% temp = readlines('input.txt')

line = "";
for i = 1:length(temp(:,1))
    line = line + temp(i,1);
end
line;
line = convertStringsToChars(line)

result = 0;
to_mult = true;

for i = 1:length(line) - 3
    if line(i) == 'd' && line(i+1) == 'o' && line(i+2) == 'n' && line(i+3) == "'" && line(i+4) == 't' && line(i+5) == '(' && line(i+6) == ')'
        to_mult = false;
    end
    if line(i) == 'd' && line(i+1) == 'o' && line(i+2) == '(' && line(i+3) == ')'
        to_mult = true;
    end
    if line(i) == 'm' && line(i+1) == 'u' && line(i+2) == 'l' && line(i+3) == '('
        i = i+4;
        num1 = [];
        while ~isempty(str2num(line(i))) && length(num1) < 3
            num1(end+1) = str2num(line(i));
            i = i+1;
        end

        num1 = vec2num(num1);

        if line(i) == ','
            i = i+1;
            num2 = [];
            while ~isempty(str2num(line(i))) && length(num2) < 3
                num2(end+1) = str2num(line(i));
                i = i+1;
            end

            num2 = vec2num(num2);

            if line(i) == ')'
                if to_mult
                    result = result + num1 * num2;
                end
            end
        end
        
    end
end

result


function num = vec2num(vec)
    num = 0;
    for i = 1:length(vec)
        num = num + vec(i) * 10^(length(vec) - i);
    end
end