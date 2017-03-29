function [ y ] = pca_power( training_file_name,test_file_name,M,iterations)

    training_data_matrix = load(training_file_name);
    [rows, cols] = size(training_data_matrix);
    training_data_matrix = training_data_matrix(:,1:cols-1);
    dimensions = cols-1;
    eigenvectors = zeros(M,dimensions);
    
    
    for j = 1:M
        b = ones(dimensions,1);
        %b(:) = rand(dimensions,1);
        cov_mat = cov(training_data_matrix);
        
        %disp(cov_mat);
        
        for i = 1:iterations
            mul = cov_mat*b;
            mag = sum(mul.^2);
            b = mul./sqrt(mag);
        end
        
        eigenvectors(j,:) = b;
        fprintf('Eigenvector %d\n',j);
        for v = 1:dimensions
            fprintf('%d: %.4f \n',v,b(v,1))
        end
        
        %update values
        for m = 1:rows
            training_data_matrix(m,:) = training_data_matrix(m,:) -  (b' * training_data_matrix(m,:)') * b';
        end
        
    end
    
    
    testing_data_matrix = load(test_file_name);
    [rows_t, cols_t] = size(testing_data_matrix);
    testing_data_matrix = testing_data_matrix(:,1:cols_t-1);
    
    
    for i = 1:rows_t
        fprintf('Test object %d \n',i);
        obj = testing_data_matrix(i,:);
        for j = 1:M
            ev = eigenvectors(j,:);
            value = obj*ev';
            fprintf('%d: %.4f \n',j,value);
        end
    end
    
end

