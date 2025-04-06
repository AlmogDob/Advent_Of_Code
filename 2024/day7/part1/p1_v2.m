clc; clear;

% data = convertStringsToChars(readlines('test_input.txt'))
data = convertStringsToChars(readlines('input.txt'))

result = 0;
for i = 1:length(data(:,1))
    temp_line = strsplit(data{i,1},':');
    cali_res = str2num(temp_line{1,1})
    eq = str2num(temp_line{1,2})
    
    if cali_true(cali_res, eq)
        result = result + cali_res;
    end
 
end

result




function possible=cali_true(cali_res, eq)
    
    possible = routine(eq, cali_res,1, '', length(eq)-1);
    
  
end

function success = routine(eq, cali_res, index, oper_array, max_len) 
    products = '+*';
    success = false;
    for pro_index = 1:length(products)
        oper_array(index) = products(pro_index);
        if (index == max_len)
            if check_eq(eq, cali_res, oper_array)
                success = true;
                return
            end
        else
            success = routine(eq, cali_res, index+1, oper_array, max_len);
            if success
                return
            end
        end
    end
end


function equal=check_eq(eq, cali_res, oper_array)
    equal = false;
    res = eq(1);
    for i = 1:length(oper_array)
        if (oper_array(i) == '+')
            res = res + eq(i+1);
        elseif (oper_array(i) == '*')
            res = res * eq(i+1);
        end
        
    end
    if (res == cali_res)
        equal = true;
    end
end


