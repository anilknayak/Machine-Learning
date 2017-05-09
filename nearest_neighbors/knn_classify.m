function [ y ] = knn_classify( training_file_name,testing_file_name,k )
    training_data_matrix = load(training_file_name);
    [tr_rows,tr_cols] = size(training_data_matrix);
    training_class = training_data_matrix(:,tr_cols);
    training_data_matrix = training_data_matrix(:,1:tr_cols-1); 

    testing_data_matrix = load(testing_file_name);
    [ts_rows,ts_cols] = size(testing_data_matrix);
    testing_class = testing_data_matrix(:,ts_cols);
    testing_data_matrix = testing_data_matrix(:,1:ts_cols-1);

    tr_mean = mean(training_data_matrix);
    tr_std = std(training_data_matrix,1);
    
    norm_training_data = (training_data_matrix-tr_mean)./tr_std;
    norm_testing_data = (testing_data_matrix-tr_mean)./tr_std;

    correct_classification = 0;
    
    for object_id = 1:ts_rows
        ts_row_data = norm_testing_data(object_id,:);
        diff_mat = norm_training_data-ts_row_data;
        power_mat = power(diff_mat,2);
        sum_vect = sum(power_mat');
        sqrt_vect = sqrt(sum_vect);
        decision_mat = [sqrt_vect' training_class];
        sort_mat = sortrows(decision_mat,1);
        sort_mat_k = sort_mat(1:k,:);
        sort_mat_k_classes = sort_mat_k(:,2);

        unique_class = unique(sort_mat_k_classes);
        true_class = testing_class(object_id);

        if size(unique_class,1) == 1
            predicted_class = unique_class(1);
            if true_class==predicted_class
                accuracy = 1;
            else
                accuracy = 0;
            end
        elseif size(unique_class,1) == k
            predicted_class = datasample(unique_class,1);
            if predicted_class == true_class
                accuracy = 1/numel(unique_class);
            else
                accuracy = 0;
            end
        else
            predicted_class = mode(sort_mat_k_classes);
            if predicted_class == true_class
                accuracy = 1;
            else
                accuracy = 0;
            end
        end
        correct_classification = correct_classification + accuracy;
        fprintf('ID=%5d, predicted=%3d, true=%3d, accuracy=%4.2f\n',object_id, predicted_class, true_class, accuracy);
    end
    fprintf('classification accuracy=%6.4f\n', correct_classification/ts_rows);
end

