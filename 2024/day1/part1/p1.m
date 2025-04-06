clc; clear; close all;

data = readmatrix('input.txt');

data(:,1) = sort(data(:,1));
data(:,2) = sort(data(:,2));
data;

dis = [];

for i = 1:length(data(:,1))
    dis(i) = abs(data(i, 1) - data(i, 2));
end
dis;
sum(dis)