clc
clear
close all
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% data=readlines('test_input.txt');
data=readlines('input.txt');

data=convertStringsToChars(data);

map = '';


for i=1:length(data(:,1))
    for j=1:length(data{1,:})
        % fprintf("%d, %d\n",i, j);
        map(i,j)=data{i,1}(j);

    end
end
empty_map = map;
% .       = empty 
% #       = block
% ^,>,v,< = guard
% X       = visited

[map,outside, isit_loop] = run_map(map);
% map
% empty_map
counter_loop = 0;
for i = 1:length(map(:,1))
    for j = 1:length(map(1,:))
        if i == 11 && j == 14
            a = 1;
        end
        fprintf("i:%d/%d | j:%d/%d\n", i, length(map(:,1)), j, length(map(1,:)))
        if contains('1234',map(i,j))
            if empty_map(i,j) == '^'
                continue;
            end
            empty_map(i,j) = '#';
                [temp_map,outside, isit_loop] = run_map(empty_map);
                if isit_loop
                    counter_loop = counter_loop + 1;
                end
            empty_map(i,j) = '.';
        end
    end
end

result = counter_loop







%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [guard_i,guard_j]=find_guard(map)

    for i=1:length(map(:,1))
        for j=1:length(map(1,:))
                
            if map(i,j)=='^' || map(i,j)=='>' || map(i,j)=='v' || map(i,j)=='<'

                guard_i=i;
                guard_j=j;
                return
            end

        end
    end
end


function [map,outside]=update_pos(map)
    
    outside=0;
    [guard_i,guard_j]=find_guard(map);

    if map(guard_i,guard_j)=='^'

        if guard_i==1

            outside=1;
            return
        end
            
        if contains('1234.',map(guard_i-1,guard_j))

            map(guard_i,guard_j)='1';
            map(guard_i-1,guard_j)='^';

        elseif map(guard_i-1,guard_j)=='#'

            map(guard_i,guard_j)='>';
            
        end

    end

    if map(guard_i,guard_j)=='>'

        if guard_j==length(map(1,:))

            outside=1;
            return
        end

        if contains('1234.',map(guard_i,guard_j+1))

            map(guard_i,guard_j)='2';
            map(guard_i,guard_j+1)='>';

        elseif map(guard_i,guard_j+1)=='#'

            map(guard_i,guard_j)='v';
            
        end

    end

    if map(guard_i,guard_j)=='v'

        if guard_i==length(map(:,1))

            outside=1;
            return
        end

        if contains('1234.',map(guard_i+1,guard_j))

            map(guard_i,guard_j)='3';
            map(guard_i+1,guard_j)='v';

        elseif map(guard_i+1,guard_j)=='#'

            map(guard_i,guard_j)='<';
            
        end

    end

    if map(guard_i,guard_j)=='<'

        if guard_j==1

            outside=1;
            return
        end

        if contains('1234.',map(guard_i,guard_j-1))

            map(guard_i,guard_j)='4';
            map(guard_i,guard_j-1)='<';

        elseif map(guard_i,guard_j-1)=='#'

            map(guard_i,guard_j)='^';
            
        end
    end
end


function [map,outside, isit_loop]=check_loop(map)
    
    outside=0;
    isit_loop = 0;
    [guard_i,guard_j]=find_guard(map);

    if map(guard_i,guard_j)=='^'

        if guard_i==1

            outside=1;
            map(guard_i,guard_j)='1';
            return
        end
            
        if contains('234.',map(guard_i-1,guard_j))

            map(guard_i,guard_j)='1';
            map(guard_i-1,guard_j)='^';

        elseif map(guard_i-1,guard_j)=='#'

            map(guard_i,guard_j)='>';

        elseif map(guard_i-1,guard_j)=='1'
              
            isit_loop = 1;
            return;

        end

    end

    if map(guard_i,guard_j)=='>'

        if guard_j==length(map(1,:))

            outside=1;
            map(guard_i,guard_j)='2';
            return
        end

        if contains('134.',map(guard_i,guard_j+1))

            map(guard_i,guard_j)='2';
            map(guard_i,guard_j+1)='>';

        elseif map(guard_i,guard_j+1)=='#'

            map(guard_i,guard_j)='v';
        
        elseif map(guard_i,guard_j+1)=='2'
              
            isit_loop = 1;
            return;
    
        end

    end

    if map(guard_i,guard_j)=='v'

        if guard_i==length(map(:,1))

            outside=1;
            map(guard_i,guard_j)='3';
            return
        end

        if contains('124.',map(guard_i+1,guard_j))

            map(guard_i,guard_j)='3';
            map(guard_i+1,guard_j)='v';

        elseif map(guard_i+1,guard_j)=='#'

            map(guard_i,guard_j)='<';
        
        elseif map(guard_i+1,guard_j)=='3'
              
            isit_loop = 1;
            return;
    
        end

    end

    if map(guard_i,guard_j)=='<'

        if guard_j==1

            outside=1;
            map(guard_i,guard_j)='4';
            return
        end

        if contains('123.',map(guard_i,guard_j-1))

            map(guard_i,guard_j)='4';
            map(guard_i,guard_j-1)='<';

        elseif map(guard_i,guard_j-1)=='#'

            map(guard_i,guard_j)='^';
        
        elseif map(guard_i,guard_j-1)=='4'
              
            isit_loop = 1;
            return;
        
        end

    end
end


function [map,outside, isit_loop] = run_map(map)

    outside = 0;
    isit_loop = 0;
    count = 0;
    while outside==0 && isit_loop==0
        count = count + 1;
        [map,outside, isit_loop]=check_loop(map);

        if (count > length(map(:,1)) * length(map(1,:)))
            isit_loop = 1;
        end
    end
end