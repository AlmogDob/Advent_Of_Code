clc; clear;

a = routine(1, '', 5)

function success = routine(index, array, max_len) 
    products = '+-*';
    success = false;
    for pro_index = 1:length(products)
        array(index) = products(pro_index);
        if (index == max_len)
            array
            if (array(1) == '-')
                success = true;
                return
            end
        else
            success = routine(index+1, array, max_len);
            if success
                return
            end
        end
    end
end

