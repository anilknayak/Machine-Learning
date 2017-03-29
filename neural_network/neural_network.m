function y = neural_network (training_file_name,testing_file_name,layers,units_per_layer,rounds)
    %Reading Training File
    training_data_matrix = load(training_file_name);
    [rows, cols] = size(training_data_matrix);
    target_value = training_data_matrix(:,cols);
    
    %Taking out Unique Class labels
    class_lables = unique(target_value);
    output_layer_perceptron = [1 ; class_lables];

    %Modifying the training file and removed the class Labels
    [number_classes,number_classes_c] = size(class_lables);
    training_data_matrix = training_data_matrix(:,1:cols-1);

    %Normalizing the Training values
    max_val_attr = max(training_data_matrix);
    max_val_entire = max(max_val_attr);
    training_data_matrix = training_data_matrix./max_val_entire;

    %Adding Bias value for X0 as 1
    training_data_matrix = [ones(size(training_data_matrix,1),1) training_data_matrix];

    %Preparing the layers and defining the number of units
    dimensions = cols-1;
    total_layers = layers;
    layer_vector_no_of_element = zeros(layers,1);
    layer_vector_no_of_element(1) = dimensions+1;

    if units_per_layer > 0 && layers > 2
        for i = 2:total_layers-1
           layer_vector_no_of_element(i) =  units_per_layer+1;
        end
    end

    layer_vector_no_of_element(total_layers) = number_classes+1;
    max_col = max(layer_vector_no_of_element);

    %Initializing the variables Weight, Z , A, delta and learning rate
    w = zeros(total_layers,max_col,max_col);
    w(:,:,:) = -0.05 + (0.05-(-0.05)).*rand(total_layers,max_col,max_col);
    z = zeros(total_layers,max_col);
    z(:,1) = 1;
    a = zeros(total_layers,max_col);
    a(:,1) = 1;
    delta = zeros(total_layers,max_col);
    delta(:,1) = 1;

    learning_rate = 1;

    %For the performance improvement, added and fit the data to defined
    %dimension
    [r1,c1] = size(z(1,:));
    [r2,c2] = size(training_data_matrix(1,:));

    if c1 > c2
        training_data_matrix = [training_data_matrix zeros(size(training_data_matrix,1),c1-c2) ];
    end


    for round = 1:rounds
        for row = 1:rows
            z(1,:) = training_data_matrix(row,:);

            %Forward Start
            %Each Layer - Each Perceptron - Z and A calculation Ends
            for layer = 2:total_layers
                number_of_perceptron_next_layer = layer_vector_no_of_element(layer);
                number_of_perceptron_previous_layer = layer_vector_no_of_element(layer-1);

                z_previous = z(layer-1,1:number_of_perceptron_previous_layer);

                for perceptron = 2:number_of_perceptron_next_layer
                    each_perceptron_weight = w(layer,perceptron,1:number_of_perceptron_previous_layer);
                    a_val = z_previous*(squeeze(each_perceptron_weight));
                    a(layer,perceptron) = a_val;
                    z(layer,perceptron) = 1/(1+exp(-1*a_val));
                end
            end
            %Each Layer - Each Perceptron - Z and A calculation Ends
            %Forward End

            %Backpropagation Start
            %Update Weights Backpropagation
            for layer = total_layers:-1:2
                number_of_perceptron_current_layer = layer_vector_no_of_element(layer);
                %Output Unit Pj delta calculation
                if layer == total_layers
                    %For Last Layer Output Backpropagation 
                    for perceptron = 2:number_of_perceptron_current_layer
                        z_output_perceptron = z(layer,perceptron);
                        if output_layer_perceptron(perceptron) == target_value(row,1)
                            target_val = 1;
                        else
                            target_val = 0;
                        end
                        delta(layer,perceptron) = (z_output_perceptron-target_val)*z_output_perceptron*(1-z_output_perceptron);
                    end
                    %Update Weights
                    %position_weight_update = sum(layer_vector_no_of_element(1:layer-1));
                    number_of_perceptron_previous_layer = layer_vector_no_of_element(layer-1);

                    for perceptron_jth = 2:number_of_perceptron_current_layer
                        delta_output = delta(layer,perceptron_jth); 
                        for perceptron_ith = 1:number_of_perceptron_previous_layer
                            z_previous_layer_perceptron = z(layer-1,perceptron_ith);
                            w(layer,perceptron_jth,perceptron_ith) = w(layer,perceptron_jth,perceptron_ith) - (learning_rate * delta_output * z_previous_layer_perceptron);
                        end
                    end
                else
                    number_of_perceptron_next_layer = layer_vector_no_of_element(layer+1);
                    for perceptron_jth = 2:number_of_perceptron_current_layer
                        z_curr_lay = z(layer,perceptron_jth);
                        sum_weight = 0;
                        for perceptron_uth = 2:number_of_perceptron_next_layer
                            sum_weight = sum_weight + (delta(layer+1,perceptron_uth) * w(layer+1,perceptron_uth,perceptron_jth));
                        end
                        delta(layer,perceptron_jth) = sum_weight * z_curr_lay * (1-z_curr_lay);
                    end
                    %Update Weights
                    number_of_perceptron_previous_layer = layer_vector_no_of_element(layer-1);

                    for perceptron_jth = 2:number_of_perceptron_current_layer
                        delta_output_1 = delta(layer,perceptron_jth); 
                        for perceptron_ith = 1:number_of_perceptron_previous_layer
                            z_previous_layer_perceptron_1 = z(layer-1,perceptron_ith);
                            w(layer,perceptron_jth,perceptron_ith) = w(layer,perceptron_jth,perceptron_ith) - (learning_rate * delta_output_1 * z_previous_layer_perceptron_1);
                        end
                    end
                end
            end
            %Updating Weight Completed Backpropagation
        end
        learning_rate = learning_rate * 0.98;
    end

    %Testing
    testing_data_matrix = load(testing_file_name);
    [rows_t, cols_t] = size(testing_data_matrix);
    actual_class = testing_data_matrix(:,cols_t);
    predict_class = zeros(size(actual_class));
    predict_class(:,1) = -1;

    testing_data_matrix = testing_data_matrix(:,1:cols_t-1);
    max_val_attr_t = max(testing_data_matrix);
    max_val_entire_t = max(max_val_attr_t);
    testing_data_matrix = testing_data_matrix./max_val_entire_t;

    testing_data_matrix = [ones(size(testing_data_matrix,1),1) testing_data_matrix];


    z_t = zeros(total_layers,max_col);
    z_t(:,1) = 1;
    z_t(total_layers,1) = 0;

    a_t = zeros(total_layers,max_col);
    a_t(:,1) = 1;

    [r1_t,c1_t] = size(z_t(1,:));
    [r2_t,c2_t] = size(testing_data_matrix(1,:));

    if c1_t > c2_t
        testing_data_matrix = [testing_data_matrix zeros(size(testing_data_matrix,1),c1_t-c2_t)];
    end

    for row = 1:rows_t
        z_t(1,:) = testing_data_matrix(row,:);
        for layer = 2:total_layers
            number_of_perceptron_next_layer_t = layer_vector_no_of_element(layer);
            number_of_perceptron_previous_layer_t = layer_vector_no_of_element(layer-1);
            for perceptron = 2:number_of_perceptron_next_layer_t
                z_previous = z_t(layer-1,1:number_of_perceptron_previous_layer_t);
                each_perceptron_weight_t = w(layer,perceptron,1:number_of_perceptron_previous_layer_t);
                a_val_t = z_previous*(squeeze(each_perceptron_weight_t));
                a_t(layer,perceptron) = a_val_t;
                z_t(layer,perceptron) = 1/(1+exp(-1*a_val_t));
            end
        end
        [M,I] = max(z_t(total_layers,:));
        if I == 1
            predict_class(row) = 0;
        else
            predict_class(row) = class_lables(I-1);
        end
    end

    correct_classification = (predict_class==actual_class);
    for row = 1:rows_t
        fprintf('ID=%5d, predicted=%3d, true=%3d, accuracy=%4.2f \n',row-1, predict_class(row), actual_class(row), correct_classification(row));
    end

    fprintf('classification accuracy=%6.4f \n', sum(correct_classification)/rows_t);
end