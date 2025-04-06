clc
clear
close all
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

data=readlines('test_input.txt');
% data=readlines('input.txt');

data=convertStringsToChars(data)

map = '';


for i=1:length(data(:,1))
    for j=1:length(data{1,:})
        % fprintf("%d, %d\n",i, j);
        map(i,j)=data{i,1}(j);

    end
end
map
% .       = empty 
% #       = block
% ^,>,v,< = guard
% X       = visited


outside=0;
while outside==0

    [map,outside]=update_pos(map);

end

map

countX=0;
for i=1:length(map(:,1))
        for j=1:length(map(1,:))

            if map(i,j)=='X'

                countX=countX+1;
            
            end

        end
end


result=countX+1

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
            
        if map(guard_i-1,guard_j)=='.' || map(guard_i-1,guard_j)=='X'

            map(guard_i,guard_j)='X';
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

        if map(guard_i,guard_j+1)=='.' || map(guard_i,guard_j+1)=='X'

            map(guard_i,guard_j)='X';
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

        if map(guard_i+1,guard_j)=='.' || map(guard_i+1,guard_j)=='X'

            map(guard_i,guard_j)='X';
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

        if map(guard_i,guard_j-1)=='.' || map(guard_i,guard_j-1)=='X'

            map(guard_i,guard_j)='X';
            map(guard_i,guard_j-1)='<';

        elseif map(guard_i,guard_j-1)=='#'

            map(guard_i,guard_j)='^';
            
        end

    end


end