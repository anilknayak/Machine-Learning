function [ y ] = svd_power( training_file_name,M,iterations)
    A = load(training_file_name);
    [rows, cols] = size(A);
    
    U = zeros(rows,M);
    S = zeros(M,M);
    V = zeros(cols,M);
    
    u_t = A * A';
    v_t = A' * A;
    
    for j = 1:M
        b_u = ones(rows,1);
        b_v = ones(cols,1);
        cov_mat_u = u_t;%cov(u_t);
        cov_mat_v = v_t;%cov(v_t);
        
        for i = 1:iterations
            mul_u = cov_mat_u*b_u;
            norm_u = sqrt(sum(mul_u.^2));
            b_u = mul_u./norm_u;
            
            mul_v = cov_mat_v*b_v;
            norm_v = sqrt(sum(mul_v.^2));
            b_v = mul_v./norm_v;
        end
        
        S(j,j) = sqrt(norm_u);
        U(:,j) = b_u;
        V(:,j) = b_v;
   
        for m = 1:rows
            u_t(m,:) = u_t(m,:) -  (b_u' * u_t(m,:)') * b_u';
        end
        
        for m = 1:cols
            v_t(m,:) = v_t(m,:) -  (b_v' * v_t(m,:)') * b_v';
        end
    end
    
    fprintf('Matrix U:\n');
    [row,col] = size(U);
    for i = 1:row
        fprintf('Row   %d:',i);
        for j = 1:col
            fprintf('%8.4f ',U(i,j));
        end
        fprintf('\n');
    end
    
    fprintf('Matrix S:\n');
    [row,col] = size(S);
    for i = 1:row
        fprintf('Row   %d:',i);
        for j = 1:col
            fprintf('%8.4f ',S(i,j));
        end
        fprintf('\n');
    end
    
    fprintf('Matrix V:\n');
    [row,col] = size(V);
    for i = 1:row
        fprintf('Row   %d:',i);
        for j = 1:col
            fprintf('%8.4f ',V(i,j));
        end
        fprintf('\n');
    end
    
    A = U*S*V';
    
    fprintf("Reconstruction (U*S*V'):\n");
    [row,col] = size(A);
    for i = 1:row
        fprintf('Row   %d:',i);
        for j = 1:col
            
            fprintf('%8.4f ',A(i,j));
      
        end
        fprintf('\n');
    end
    
   
    
end

