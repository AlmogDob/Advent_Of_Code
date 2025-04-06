clc; clear; close all;

% data = readlines("test_input.txt");
data = readlines("input.txt")

for i = 1:length(data(:, 1))
    lines(i,:) = convertStringsToChars(data(i));
end
lines

count = 0;

for i = 1:length(lines(:,1)) -3
    for j = 1:length(lines(1,:)) -3
        if check([lines(i, j) , lines(i+1, j+1) , lines(i+2, j+2) , lines(i+3, j+3)])    % diagonal right down
            count = count + 1;
        end
    end
end


for i = 1:length(lines(:,1))
    for j = 1:length(lines(1,:)) -3
        if check([lines(i, j) , lines(i, j+1) , lines(i, j+2) , lines(i, j+3)])    % horziontal
            count = count + 1;
        end
    end
end
for i = 1:length(lines(:,1)) -3
    for j = 1:length(lines(1,:))
        if check([lines(i, j) , lines(i+1, j) , lines(i+2, j) , lines(i+3, j)])    % vertical
            count = count + 1;
        end
    end
end
for i = 1:length(lines(:,1))-3
    for j = 4:length(lines(1,:))
        if check([lines(i, j) , lines(i+1, j-1) , lines(i+2, j-2) , lines(i+3, j-3)])    % diagonal left down
            count = count + 1;
        end
    end
end

count


function belong = check(array)
    if (array(1) == 'X' && array(2) == 'M' && array(3) == 'A' && array(4) == 'S') || (array(1) == 'S' && array(2) == 'A' && array(3) == 'M' && array(4) == 'X')
        belong = true;
    else
        belong = false;
    end
end