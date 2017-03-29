function [ y ] = pca_power_extended( training_file_name,test_file_name,M,iterations)
  
    training_data_matrix = load(training_file_name);
    [rows, cols] = size(training_data_matrix);
    training_data_matrix = training_data_matrix(:,1:cols-1);
    dimensions = cols-1;
    cov_mat = cov(training_data_matrix);
    
    eigenvectors = zeros(M,dimensions);
    
    prev_mag = 0.0;
    
    
    fprintf('Stoping Criteria for eigen vectors. Difference in normalizing factor < 0.000001 \n');
    
    for j = 1:M
        %for i = 1:iterations
        flag = 1;
        counter = 0;
        
        b = ones(dimensions,1);
        b(:) = rand(dimensions,1);
        
        while flag
            %disp(max(b));
            mul = cov_mat*b;
            %max_element = max(mul);
            mag = sum(mul.^2);
            b = mul./mag;
                       
            if mod(counter,2) == 1
                %fprintf('Prev %f Max Element %f diff %f at iteration %d \n',prev_mag,max_element,abs(prev_mag - max_element),counter);
                if abs(prev_mag - mag) < 0.000001
                    flag = 0;
                else
                    prev_mag = mag;
                    %fprintf('Prev %f max element %f\n',prev_mag,max_element);
                end
            end
            
            counter = counter + 1;
            
            %if counter == 100
            %   flag = 0; 
            %end
            
        end
        
        fprintf('For Eigenvector %d took %d number of Iterations \n',j,counter);
        
        eigenvectors(j,:) = b;
        fprintf('Eigenvector %d\n',j);
        for v = 1:dimensions
            fprintf('%d: %.4f \n',v,b(v,1))
        end
        
        
    end
    
    
    testing_data_matrix = load(test_file_name);
    [rows_t, cols_t] = size(testing_data_matrix);
    testing_data_matrix = testing_data_matrix(:,1:cols_t-1);
    
    
    for i = 1:10
        fprintf('Test object %d \n',i);
        obj = testing_data_matrix(i,:);
        for j = 1:M
            ev = eigenvectors(j,:);
            value = obj*ev';
            fprintf('%d: %.4f \n',j,value);
        end
    end
    
end

