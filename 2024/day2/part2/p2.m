clc; clear; close all;

% data = readmatrix('test_input.txt')
data = readmatrix('input.txt');

counter = 0;

for report = 1:length(data(:,1))
    safe = true;
    current_sign = 0;
    fprintf("------------------------------------\n")
    for level = 1:length(data(1,:))-1
        current = data(report, level);
        next    = data(report, level+1);
        dis = next - current;
        if isnan(current) || isnan(next)
            break
        end
        
        fprintf('%d, %d --> %d\n',current, next, dis);
        if 0 == dis || abs(dis) > 3
            safe = false;
            break
        end 
        if 0 == current_sign
            current_sign = sign(dis);
        end
        if dis * current_sign < 0
            safe = false;
            break
        end
    end
    % break
    counter = counter + 1*safe;
end

counter
