clc; clear; close all;

% data = fopen("test_input.txt");
data = fopen("input.txt");
rules = [];
updates = {};

while true
    temp_line = fgetl(data);
    if isempty(temp_line)
        break;
    end
    temp_numbers = sscanf(temp_line, '%d|%d', 2);
    rules(end+1,:) = temp_numbers;
end
while true
    temp_line = fgetl(data);
    if isempty(temp_line)
        break;
    end
    updates{end+1,1} = str2num(temp_line);
end

rules
updates

incorrect_updates = {};

for i = 1:length(updates(:,1))
    if ~update_check(updates{i, 1}, rules)
        incorrect_updates{end+1, 1} = updates{i,1};
    end
end

for index = 1:length(incorrect_updates(:, 1))
    while true
        [is_correct,i,j] = update_check(incorrect_updates{index, 1}, rules);
        if is_correct
            break;
        end
        temp = incorrect_updates{index, 1}(i);
        incorrect_updates{index, 1}(i) = incorrect_updates{index, 1}(j);
        incorrect_updates{index, 1}(j) = temp;
    end
end


result = 0;
for i = 1:length(incorrect_updates(:,1))
    result = result + incorrect_updates{i,1}((length(incorrect_updates{i,1})+1)/2);
end

result





function is_it_rule = rule_check(rules, num1, num2)
    is_it_rule = false;
    for i = 1:length(rules(:, 1))
        if (rules(i, 1) == num1 && rules(i, 2) == num2)
            is_it_rule = true;
            break;
        end
    end
end

function [is_correct,i,j] = update_check(update, rules) 
    is_correct = true;
    for i = 1:length(update)-1
        for j = i+1:length(update)
            if (rule_check(rules, update(j), update(i)))
                is_correct = false;
                return;
            end
        end
    end
end