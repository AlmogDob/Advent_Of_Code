clc; clear; close all;

data = readmatrix('input.txt');

sim_score = 0;

for left = 1:length(data(:,1))
    counter = 0;
    for right = 1:length(data(:,2))
        if data(right,2) == data(left,1)
            counter = counter + 1;
        end
    end
    sim_score = sim_score + data(left,1)*counter;
end

sim_score