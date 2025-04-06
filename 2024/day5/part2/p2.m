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

correct_updates = {};

for i = 1:length(updates(:,1))
    if update_check(updates{i, 1}, rules)
        correct_updates{end+1, 1} = updates{i,1};
    end
end

result = 0;
for i = 1:length(correct_updates(:,1))
    result = result + correct_updates{i,1}((length(correct_updates{i,1})+1)/2);
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

function is_correct = update_check(update, rules) 
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