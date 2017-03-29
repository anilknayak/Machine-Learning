function y = logistic_regression(name,training_file_name,degree,testing_file_name)
    training_data_matrix = load(training_file_name);
    [rows, cols] = size(training_data_matrix);

    %actuall output vector
    tn_col_vector = training_data_matrix(:,cols);
    tn_col_vector(tn_col_vector>1)=0;
    
    phi_matrix = zeros(rows, 1+((degree)*(cols-1)));
        
    for row = 1: rows
        phi_matrix(row,1) = 1;
        col_index = 2;
        for col = 1: cols-1
            for deg = 1: degree
                phi_matrix(row, col_index) = training_data_matrix(row, col)^deg;
                col_index = col_index + 1;
            end
        end
    end
    
    [phi_rows,phi_cols] = size(phi_matrix);
    
    W = zeros(phi_cols, 1);
    %weight_matrix_col_vector = weight_matrix';
    
    %disp(weight_matrix_col_vector)
    
    %tn_col_vector
    %weight_matrix_col_vector
    %phi_matrix
    end_flag = true;
    
    
    counter = 1;
    
    while end_flag
        %R is NxN
        %Yn is Nx1
        %Tn is Nx1
        %PHI is NxD+1
        %W is D+1x1
        %H is D+1xD+1
        %E is D+1xN
        %W_NEW 
        %estimate_probability is 
        
        %1xD+1 * D+1xN
        estimate_probability = transpose(W)*transpose(phi_matrix);
        yn_exp = 1+exp(-estimate_probability);
        yn_row = 1 ./ yn_exp;
        
        yn_col = yn_row';
        %disp(yn_col);
        %error = (yn-tn_col_vector)
        [rows_yn,cols_yn] = size(yn_col);
        R = zeros(rows_yn,rows_yn);
        
        for i = 1:rows_yn
            calc = yn_col(i,1)*(1-yn_col(i,1));
            R(i,i) = calc;
            %disp(calc);
        end
                
        %D+1xN  * NxN * NxD+1
        H = transpose(phi_matrix)*R*phi_matrix;
        
        %D+1xN * Nx1
        E = transpose(phi_matrix)*(yn_col-tn_col_vector);
        
        %D+1x1 - (D+1xD+1 * D+1*N * Nx1)
        W_NEW = W - (pinv(H)*transpose(phi_matrix)*(yn_col-tn_col_vector));
        
        %D+1x1
        W_DIFF = W-W_NEW;
        ABS_SUM_W_DIFF = abs(sum(W_DIFF));
        
        %Check for weight to stop the iteration
        if ABS_SUM_W_DIFF < 0.001
            end_flag = false;
        end
        
        %Check for cross entropy error to stop the iteration
        e1 = transpose(W_NEW)*transpose(phi_matrix);
        y1 = 1+exp(-e1);
        y2 = 1 ./ y1;
        y3 = y2';
        
        %D+1xN * Nx1
        E_NEW = transpose(phi_matrix)*(y3-tn_col_vector);
        E_DIFF = E-E_NEW;
        ABS_SUM_E_DIFF = abs(sum(E_DIFF));
        
        %Check for weight to stop the iteration
        if ABS_SUM_E_DIFF < 0.001
            end_flag = false;
        end
        
        W = W_NEW;
        
        counter = counter + 1;
    end

    [w_row,w_col] = size(W);
    
    for i = 1:w_row
       fprintf('w%d=%.4f \n',i-1, W(i)); 
    end
    
    
    %Classification Starts from here for the testing data
    testing_data_matrix = load(testing_file_name);
    
    [t_rows, t_cols] = size(testing_data_matrix);
    
    %actuall output vector
    ts_cls_col_vector = testing_data_matrix(:,t_cols);
    ts_cls_col_vector(ts_cls_col_vector>1)=0;
    
    t_phi_matrix = zeros(t_rows, 1+((degree)*(t_cols-1)));
        
    for row = 1: t_rows
        t_phi_matrix(row,1) = 1;
        col_index = 2;
        for col = 1: t_cols-1
            for deg = 1: degree
                t_phi_matrix(row, col_index) = testing_data_matrix(row, col)^deg;
                col_index = col_index + 1;
            end
        end
    end
    
    [t_phi_rows,t_phi_cols] = size(t_phi_matrix);
    
    %1xD+1 * D+1xN
    t_estimate_prob = transpose(W)*transpose(t_phi_matrix);
    t_yn = 1+exp(-t_estimate_prob);
    t_yn_row = 1 ./ t_yn;
    t_yn_col = t_yn_row';
    
    
    
    %t_estimate_prob  WxPhi
    %t_yn_col  Prob
    t_estimate_prob(t_estimate_prob>0)=1;
    t_estimate_prob(t_estimate_prob<0)=0;
    
    t_estimate_prob = t_estimate_prob';
    number_of_true_predicted_class = 0;
    format = 'ID=%5d, predicted=%3d, probability = %.4f, true=%3d, accuracy=%4.2f \n';
    %fileID = fopen('result.txt','w');
    for row = 1: t_rows
       predicted = t_estimate_prob(row);
       probability = t_yn_col(row);
       actual = ts_cls_col_vector(row);
       accuracy = 0.0;
       
       if predicted==0
            probability = 1-probability;
       end
       
       if predicted == actual
            accuracy = 1.0;
            number_of_true_predicted_class = number_of_true_predicted_class + 1;
       end
       
       fprintf(format, row-1, predicted, probability,actual, accuracy);
    end
   
    classification_accuracy = number_of_true_predicted_class/t_rows;
    fprintf('classification accuracy=%6.4f \n', classification_accuracy);
    %fclose(fileID);
end