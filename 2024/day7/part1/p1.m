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
    possible = false;
    num_space = length(eq) - 1;
    operationas = '+*';
    oper_array = '';

    for max_num_of_mul = 0:num_space
        for num_of_mul = 0:max_num_of_mul
            if num_of_mul == 0
                for space_index = 1:num_space
                    oper_array(space_index) = operationas(1);
                end
            elseif num_of_mul == num_space
                for space_index = 1:num_space
                    oper_array(space_index) = operationas(2);
                end
            end


            if max_num_of_mul == 1
                for space_index = 1:num_space
                    for temp_index = 1:num_space
                        oper_array(temp_index) = operationas(1);
                    end
                    oper_array(space_index) = operationas(2);
                    if check_eq(eq, cali_res, oper_array)
                        possible = true;
                        return
                    end 
                end
            end
            if max_num_of_mul == 2
                for space_index1 = 1:num_space
                    for temp_index = 1:num_space
                        oper_array(temp_index) = operationas(1);
                    end
                    oper_array(space_index1) = operationas(2);
                    for space_index2 = space_index1:num_space
                        oper_array(space_index2) = operationas(2);
                        if check_eq(eq, cali_res, oper_array)
                            possible = true;
                            return
                        end 
                    end
                    
                end
            end
            if max_num_of_mul == 3
                for space_index1 = 1:num_space

                    for space_index2 = space_index1:num_space

                        for space_index3 = space_index2:num_space
                            for temp_index = 1:num_space
                                oper_array(temp_index) = operationas(1);
                            end

                            oper_array(space_index1) = operationas(2);
                            oper_array(space_index2) = operationas(2);
                            oper_array(space_index3) = operationas(2);
                            
                            if check_eq(eq, cali_res, oper_array)
                                possible = true;
                                return
                            end  
                        end
                    end
                    
                end
            end
            if max_num_of_mul == 4
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space

                        for space_index3 = space_index2:num_space

                            for space_index4 = space_index3:num_space

                                for temp_index = 1:num_space
                                    oper_array(temp_index) = operationas(1);
                                end

                                oper_array(space_index1) = operationas(2);
                                oper_array(space_index2) = operationas(2);
                                oper_array(space_index3) = operationas(2);
                                oper_array(space_index4) = operationas(2);
                                
                                if check_eq(eq, cali_res, oper_array)
                                    possible = true;
                                    return
                                end    
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 5
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    for temp_index = 1:num_space
                                        oper_array(temp_index) = operationas(1);
                                    end

                                    oper_array(space_index1) = operationas(2);
                                    oper_array(space_index2) = operationas(2);
                                    oper_array(space_index3) = operationas(2);
                                    oper_array(space_index4) = operationas(2);
                                    oper_array(space_index5) = operationas(2);
                                    
                                    if check_eq(eq, cali_res, oper_array)
                                        possible = true;
                                        return
                                    end 
                                end
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 6
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    
                                    for space_index6 = space_index5:num_space
                                        for temp_index = 1:num_space
                                            oper_array(temp_index) = operationas(1);
                                        end
    
                                        oper_array(space_index1) = operationas(2);
                                        oper_array(space_index2) = operationas(2);
                                        oper_array(space_index3) = operationas(2);
                                        oper_array(space_index4) = operationas(2);
                                        oper_array(space_index5) = operationas(2);
                                        oper_array(space_index6) = operationas(2);
                                        
                                        if check_eq(eq, cali_res, oper_array)
                                            possible = true;
                                            return
                                        end 
                                    end
                                end
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 7
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    
                                    for space_index6 = space_index5:num_space

                                        for space_index7 = space_index6:num_space
                                            for temp_index = 1:num_space
                                                oper_array(temp_index) = operationas(1);
                                            end
        
                                            oper_array(space_index1) = operationas(2);
                                            oper_array(space_index2) = operationas(2);
                                            oper_array(space_index3) = operationas(2);
                                            oper_array(space_index4) = operationas(2);
                                            oper_array(space_index5) = operationas(2);
                                            oper_array(space_index6) = operationas(2);
                                            oper_array(space_index7) = operationas(2);
                                            
                                            if check_eq(eq, cali_res, oper_array)
                                                possible = true;
                                                return
                                            end 
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 8
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    
                                    for space_index6 = space_index5:num_space

                                        for space_index7 = space_index6:num_space

                                            for space_index8 = space_index7:num_space
                                                for temp_index = 1:num_space
                                                    oper_array(temp_index) = operationas(1);
                                                end
            
                                                oper_array(space_index1) = operationas(2);
                                                oper_array(space_index2) = operationas(2);
                                                oper_array(space_index3) = operationas(2);
                                                oper_array(space_index4) = operationas(2);
                                                oper_array(space_index5) = operationas(2);
                                                oper_array(space_index6) = operationas(2);
                                                oper_array(space_index7) = operationas(2);
                                                oper_array(space_index8) = operationas(2);
                                                
                                                if check_eq(eq, cali_res, oper_array)
                                                    possible = true;
                                                    return
                                                end 
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 9
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    
                                    for space_index6 = space_index5:num_space

                                        for space_index7 = space_index6:num_space

                                            for space_index8 = space_index7:num_space

                                                for space_index9 = space_index8:num_space
                                                    for temp_index = 1:num_space
                                                        oper_array(temp_index) = operationas(1);
                                                    end
                
                                                    oper_array(space_index1) = operationas(2);
                                                    oper_array(space_index2) = operationas(2);
                                                    oper_array(space_index3) = operationas(2);
                                                    oper_array(space_index4) = operationas(2);
                                                    oper_array(space_index5) = operationas(2);
                                                    oper_array(space_index6) = operationas(2);
                                                    oper_array(space_index7) = operationas(2);
                                                    oper_array(space_index8) = operationas(2);
                                                    oper_array(space_index9) = operationas(2);
                                                    
                                                    if check_eq(eq, cali_res, oper_array)
                                                        possible = true;
                                                        return
                                                    end 
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 10
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    
                                    for space_index6 = space_index5:num_space

                                        for space_index7 = space_index6:num_space

                                            for space_index8 = space_index7:num_space

                                                for space_index9 = space_index8:num_space

                                                    for space_index10 = space_index9:num_space
                                                        for temp_index = 1:num_space
                                                            oper_array(temp_index) = operationas(1);
                                                        end
                    
                                                        oper_array(space_index1) = operationas(2);
                                                        oper_array(space_index2) = operationas(2);
                                                        oper_array(space_index3) = operationas(2);
                                                        oper_array(space_index4) = operationas(2);
                                                        oper_array(space_index5) = operationas(2);
                                                        oper_array(space_index6) = operationas(2);
                                                        oper_array(space_index7) = operationas(2);
                                                        oper_array(space_index8) = operationas(2);
                                                        oper_array(space_index9) = operationas(2);
                                                        oper_array(space_index10) = operationas(2);
                                                        
                                                        if check_eq(eq, cali_res, oper_array)
                                                            possible = true;
                                                            return
                                                        end 
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
            if max_num_of_mul == 11
                for space_index1 = 1:num_space
                    
                    for space_index2 = space_index1:num_space
                        
                        for space_index3 = space_index2:num_space
                            
                            for space_index4 = space_index3:num_space
                                
                                for space_index5 = space_index4:num_space
                                    
                                    for space_index6 = space_index5:num_space

                                        for space_index7 = space_index6:num_space

                                            for space_index8 = space_index7:num_space

                                                for space_index9 = space_index8:num_space

                                                    for space_index10 = space_index9:num_space

                                                        for space_index11 = space_index10:num_space
                                                            for temp_index = 1:num_space
                                                                oper_array(temp_index) = operationas(1);
                                                            end
                        
                                                            oper_array(space_index1) = operationas(2);
                                                            oper_array(space_index2) = operationas(2);
                                                            oper_array(space_index3) = operationas(2);
                                                            oper_array(space_index4) = operationas(2);
                                                            oper_array(space_index5) = operationas(2);
                                                            oper_array(space_index6) = operationas(2);
                                                            oper_array(space_index7) = operationas(2);
                                                            oper_array(space_index8) = operationas(2);
                                                            oper_array(space_index9) = operationas(2);
                                                            oper_array(space_index10) = operationas(2);
                                                            oper_array(space_index11) = operationas(2);
                                                            
                                                            if check_eq(eq, cali_res, oper_array)
                                                                possible = true;
                                                                return
                                                            end 
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
    
    
            if check_eq(eq, cali_res, oper_array)
                possible = true;
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