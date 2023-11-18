function main()
    % Import block definitions
    types = block_types();
    blocklist = [types.simple_block types.double_block types.corner_block types.square_block];
    
    % Random placement with abort criterium 
    rng(0,'twister');
    abortCounter = 0;
    abortScore = 0;
    best_configuration = [];
    while 1
        for item_idx = 1:length(blocklist)
            blocklist(item_idx).position = randi([-3 3],1,2);
            blocklist(item_idx).rotation = randi([0 3],1,1);
        end
    
        if ~checkBuildValidity(blocklist) continue; end
        
        score = objectiveFunction(blocklist);
        if score > abortScore
            abortScore = score;
            abortCounter = 0;
            best_configuration = blocklist;
            disp("Current Best Score:");
            disp(score);
        else
            abortCounter = abortCounter + 1;
        end
    
        if abortCounter > 1e4; break; end
    end
    blocklist = best_configuration;
    
    % Visualize
    disp("Validity:");
    disp(checkBuildValidity(blocklist));
    disp("Configuration Score:");
    disp(objectiveFunction(blocklist));
    drawBag(blocklist);
end

function value = objectiveFunction(blocklist)
    value = 0;
    adjacency_matrix = checkBlockConnections(blocklist);
    for item_idx = 1:length(blocklist)
        item_multiplier = 1;
        % Check for modifiers from other items
        for i = 1:length(blocklist)
            if adjacency_matrix(item_idx, i) ~= 1; continue; end
            item_multiplier = item_multiplier * blocklist(i).connection_modifier;
        end
        value = value + (blocklist(item_idx).value * item_multiplier);
    end
end 

function adjacency_matrix = checkBlockConnections(blocklist)
    adjacency_matrix = zeros(length(blocklist));
    % j block that connects, i block that is connected to
    % Connections are one-directional 
    for j = 1:length(blocklist)
        for i = 1:length(blocklist)
            if i==j; continue; end
            % Check each possible connection location of j
            for connection_idx = 1:length(blocklist(j).connection_blocks) 
                connection_block_position = blocklist(j).position + rotateCoords(blocklist(j).connection_blocks(connection_idx,:), blocklist(j).rotation);
                % Check for overlap with each block of item i
                for k = 1:size(blocklist(i).blocks,1)
                    if connection_block_position == blocklist(i).position + rotateCoords(blocklist(i).blocks(k,:), blocklist(i).rotation);
                        adjacency_matrix(i,j) = 1;
                        break;
                    end
                end
            end
        end
    end
end

% Disjointness of n axis-parallel rectangles can be checked in O(n log n) time
% This is certainly much worse.
function isValid = checkBuildValidity(blocklist)
    % Check for overlap in item positions
    placed_blocks = [];
    for i = 1:length(blocklist)
        for k = 1:size(blocklist(i).blocks,1)
            new_block = blocklist(i).position + rotateCoords(blocklist(i).blocks(k,:), blocklist(i).rotation);
            if checkIfInCoordArray(new_block, placed_blocks)
                isValid = false;
                return
            end
            placed_blocks = [placed_blocks; new_block];
        end
    end
    isValid = true;
end

function isInArray = checkIfInCoordArray(new_coords, existing_coords)
    for i = 1:size(existing_coords,1)
        if new_coords == existing_coords(i,:)
            isInArray = true;
            return 
        end
    end
    isInArray = false;
end

function new_coords = rotateCoords(position, rotation)
    % 1=90degrees right 2=180degrees right etc
    rad = (-1)*double(rotation)*pi/2;
    rotation_matrix = [cos(rad) -sin(rad); sin(rad) cos(rad)];
    new_coords = transpose(int32(rotation_matrix * transpose(double(position))));
end

function drawBag(blocklist)
% Produce a plot of all items
    hold on;
    for i = 1:length(blocklist)
        placed_blocks = [];
        for k = 1:size(blocklist(i).blocks,1)
            new_block = blocklist(i).position + rotateCoords(blocklist(i).blocks(k,:), blocklist(i).rotation);
            placed_blocks = [placed_blocks; new_block];
        end
        scatter(placed_blocks(:,1),placed_blocks(:,2), 5000,'filled',"square");
        xlim([-6 6]);
        ylim([-6 6]);
        axis equal;
    end
end