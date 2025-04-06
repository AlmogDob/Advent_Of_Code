clc; clear; close all;

% data = readlines("test_input.txt");
data = readlines("input.txt")

for i = 1:length(data(:, 1))
    lines(i,:) = convertStringsToChars(data(i));
end
lines

count = 0;

for i = 2:length(lines(:,1))-1
    for j = 2:length(lines(1,:))-1
        if lines(i, j) == 'A'
            if (lines(i-1, j+1) == 'S' && lines(i+1, j-1) == 'M') || (lines(i-1, j+1) == 'M' && lines(i+1, j-1) == 'S')
                if (lines(i-1, j-1) == 'S' && lines(i+1, j+1) == 'M') || (lines(i-1, j-1) == 'M' && lines(i+1, j+1) == 'S')
                    count = count + 1;
                end
            end
        end
    end
end

count