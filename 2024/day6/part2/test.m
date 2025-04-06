clc
clear
close all

%% Read input data
% data = readlines('test_input.txt');
data = readlines('input.txt');
data = convertStringsToChars(data);

map = '';
for i = 1:length(data(:,1))
    for j = 1:length(data{1,:})
        map(i, j) = data{i,1}(j);
    end
end
empty_map = map;

% Run initial simulation to mark visited positions
[map, outside, isit_loop, visited_positions] = run_map(map);

counter_loop = 0;
for idx = 1:size(visited_positions, 1)
    i = visited_positions(idx, 1);
    j = visited_positions(idx, 2);
    
    if empty_map(i, j) == '^' || empty_map(i, j) == '>' || empty_map(i, j) == 'v' || empty_map(i, j) == '<'
        continue; % Don't block guard's start position
    end
    
    fprintf("Testing obstacle at i:%d, j:%d\n", i, j);
    temp_map = empty_map; % Ensure a fresh copy each time
    temp_map(i, j) = '#';
    [~, ~, is_loop] = run_map(temp_map);
    
    if is_loop
        counter_loop = counter_loop + 1;
    end
end

result = counter_loop;

%% Function to find guard position
function [guard_i, guard_j] = find_guard(map)
    [guard_i, guard_j] = find(ismember(map, '^>v<'), 1);
end

%% Function to run simulation and track visited positions
function [map, outside, isit_loop, visited_positions] = run_map(map)
    visited_positions = [];
    outside = 0;
    isit_loop = 0;
    seen_states = containers.Map('KeyType', 'char', 'ValueType', 'logical');
    
    while outside == 0 && isit_loop == 0
        [map, outside, isit_loop, guard_i, guard_j, direction] = check_loop(map);
        state_key = sprintf('%d_%d_%s', guard_i, guard_j, direction);
        
        if isKey(seen_states, state_key)
            isit_loop = 1;
            return;
        else
            seen_states(state_key) = true;
            visited_positions = [visited_positions; guard_i, guard_j];
        end
    end
end

%% Function to check guard movement and loop detection
function [map, outside, isit_loop, guard_i, guard_j, direction] = check_loop(map, visited_positions)

    outside = 0;
    isit_loop = 0;
    [guard_i, guard_j] = find_guard(map);
    direction = map(guard_i, guard_j); % Get guard's current direction

    % Define movement offsets for each direction
    directions = {'^', '>', 'v', '<'};
    move_offsets = [-1, 0; 0, 1; 1, 0; 0, -1]; 

    % Find the direction index
    dir_index = find(strcmp(directions, direction));

    % Compute new position
    new_i = guard_i + move_offsets(dir_index, 1);
    new_j = guard_j + move_offsets(dir_index, 2);

    % Check bounds
    if new_i < 1 || new_j < 1 || new_i > size(map, 1) || new_j > size(map, 2)
        outside = 1;
        return;
    end

    % Check if revisiting a state
    state = [new_i, new_j, dir_index]; % Position + direction
    if ismember(state, visited_positions, 'rows')
        isit_loop = 1;
        return;
    end

    % Update map and store visited position
    map(guard_i, guard_j) = num2str(dir_index); % Mark path with direction index
    map(new_i, new_j) = direction; % Move guard
    visited_positions = [visited_positions; state]; % Append to visited

end
