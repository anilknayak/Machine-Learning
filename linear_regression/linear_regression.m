function y = linear_regression(name,filename,degree,lambda)
    data_matrix = load(filename);
    [rows, cols] = size(data_matrix);
    %{As the Phi(0) to Phi(M-1)}%
    phi_matrix = zeros(rows, degree+1);
    %{Calculate Phi matrix for all the degree}%
    %{if Degree is 1 then phi matrix will have 1+1 column}%
    for row = 1: rows
        %{As the Phi(0) is 1 initialized}%
        phi_matrix(row, 1) = 1;
        %{Calculate Phi matrix for all the degree}% 
        %{if Degree is 1 then phi matrix will have 1+1 column}%
        for deg = 1: degree
            phi_matrix(row, deg+1) = data_matrix(row, 1)^deg;
        end
    end
    if lambda == 0
        w = ((inv((transpose(phi_matrix) * (phi_matrix)))) * ((transpose(phi_matrix) * data_matrix(:,2))));
    else 
        w = (inv((lambda * eye(degree+1)) + (transpose(phi_matrix)* (phi_matrix)))) * (transpose(phi_matrix)) * (data_matrix(:,2)) ; 
    end 
    if degree == 1
        w(3) = 0;
    end
    fprintf('w0=%.4f \n', w(1));
    fprintf('w1=%.4f \n', w(2));
    fprintf('w2=%.4f \n', w(3));
end