simple_block = item();
simple_block.blocks = [0 0];
simple_block.connection_blocks = [];
simple_block.value = 1;
simple_block.connection_modifier = 1;

double_block = item();
double_block.blocks = [0 0; 0 1];
double_block.connection_blocks = [0 -1; 0 2];
double_block.value = 1;
double_block.connection_modifier = 2;

corner_block = item();
corner_block.blocks = [0 0; 0 1; 1 1];
corner_block.connection_blocks = [0 -1; 2 1];
corner_block.value = 1;
corner_block.connection_modifier = 3;

blocklist = [simple_block double_block corner_block];

% Constructive placement

% Test placement
blocklist(1).position = [0 0];
blocklist(1).rotation = 0;
blocklist(2).position = [0 1];
blocklist(2).rotation = 0;
blocklist(3).position = [-1 0];
blocklist(3).rotation = 3;

disp("Validity:");
disp(checkBuildValidity(blocklist));
drawBag(blocklist);
disp("Configuration Value:");
disp(objectiveFunction(blocklist));

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
                connection_block_position = blocklist(j).position + rotateBlock(blocklist(j).connection_blocks(connection_idx,:), blocklist(j).rotation);
                % Check for overlap with each block of item i
                for k = 1:size(blocklist(i).blocks,1)
                    if connection_block_position == blocklist(i).position + rotateBlock(blocklist(i).blocks(k,:), blocklist(i).rotation);
                        adjacency_matrix(i,j) = 1;
                        break;
                    end
                end
            end
        end
    end
end

function isValid = checkBuildValidity(blocklist)
    % Check for overlap in item positions
    placed_blocks = [];
    for i = 1:length(blocklist)
        for k = 1:size(blocklist(i).blocks,1)
            new_block = blocklist(i).position + rotateBlock(blocklist(i).blocks(k,:), blocklist(i).rotation);
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

function new_coords = rotateBlock(position, rotation)
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
            new_block = blocklist(i).position + rotateBlock(blocklist(i).blocks(k,:), blocklist(i).rotation);
            placed_blocks = [placed_blocks; new_block];
        end
        scatter(placed_blocks(:,1),placed_blocks(:,2));
    end
end