function main()
    % Import block definitions
    item_types = items();
    bag_types = bags();
    blocklist = [item_types.simple_block item_types.double_block item_types.corner_block item_types.square_block];
    baglist = [bag_types.leather_bag bag_types.ranger_bag bag_types.potion_belt];
    
    best_configuration = simulatedAnnealing(blocklist, baglist);

    assert(~isempty(best_configuration), "No valid configuration found.");

    heatmap(transpose(best_configuration));
end

function best_configuration = simulatedAnnealing(blocklist, baglist)
    % Find random valid configuration
    temperature = 1;
    best_configuration = [];
    [initial_configuration,blocklist,baglist] = randomPlacement(blocklist, baglist, 1e3);

    best_score = countValidConnections(blocklist, initial_configuration);

    while temperature > 1e-6
        % The maximum random movement changes with temperature
        d_limits = int(temperature * [9 7]); 

        % Either randomly move a block or rotate it
        old_blocklist = blocklist;
        block_idx = randi([1 length(blocklist)],1);
        block = blocklist(block_idx);
        if randi([0 1],1)
            block.position(1) = max(9,min(1, block.position(1) + randi([-d_limits d_limits],1)));
            block.position(2) = max(7,min(1, block.position(2) + randi([-d_limits d_limits],1)));
        else
            block.rotation = randi([0 3],1);
        end

        % Evaluate the resulting configuration
        new_placement_matrix = generatePlacementMatrix(blocklist, baglist);

        % Reset the configuration if it became invalid
        if isempty(new_placement_matrix)
            blocklist = old_blocklist;
            continue
        end

        % Check if the configuration is better
        new_score = countValidConnections(blocklist, placement_matrix);

        if new_score > best_score
            best_configuration = new_placement_matrix;
            best_score = new_score;
            disp("Current Best Score:");
            disp(best_score);
            temperature = temperature*0.95;
        end
    end
end

function [best_configuration,blocklist,baglist] = randomPlacement(blocklist, baglist, maximum_tries)
    rng(0,'twister');
    abortCounter = 0;
    abortScore = 0;
    best_configuration = [];

    tic;
    while 1
        for item_idx = 1:length(blocklist)
            blocklist(item_idx).position(1) = randi([0 8],1);
            blocklist(item_idx).position(2) = randi([0 6],1);
            blocklist(item_idx).rotation = randi([0 3],1,1);
        end
        for bag_idx = 1:length(baglist)
            baglist(bag_idx).position = randi([0 6],1,2);
            baglist(bag_idx).rotation = randi([0 3],1,1);
        end
        placement_matrix = generatePlacementMatrix(blocklist, baglist);

        % Check for invalid configuration
        if isempty(placement_matrix)
            abortCounter = abortCounter + 1;
            continue;
        end

        score = countValidConnections(blocklist, placement_matrix);
        if score > abortScore
            abortScore = score;
            best_configuration = placement_matrix;
            disp("Current Best Score:");
            disp(score);
            disp("Average time per evaluation:")
            disp(toc/abortCounter);
            abortCounter = 0;
            tic;
        end
    
        if abortCounter > maximum_tries && ~isempty(best_configuration); break; end
    end
end

function placement_matrix = generatePlacementMatrix(blocklist, baglist)
    grid_size = [9 7]; % This is the maximum grid from the game
    placement_matrix = zeros(grid_size(1),grid_size(2)) - 1;

    % Place Bags
    for bag_idx = 1:length(baglist)
        for block_idx = 1:size(baglist(bag_idx).blocks,1)

            new_block = baglist(bag_idx).position + ...
                rotateCoords(baglist(bag_idx).blocks(block_idx,:), baglist(bag_idx).rotation);

            if (new_block(1) < 1 || new_block(2) < 1 || ...
                new_block(1) > grid_size(1) || new_block(2) > grid_size(2) || ...
                placement_matrix(new_block(1), new_block(2)) ~= -1)
                % Invalid placement
                placement_matrix = [];
                return;
            end

            placement_matrix(new_block(1), new_block(2)) = 0;
        end
    end

    % Place Items
    for item_idx = 1:length(blocklist)
        for block_idx = 1:size(blocklist(item_idx).blocks,1)

            new_block = blocklist(item_idx).position + ...
                rotateCoords(blocklist(item_idx).blocks(block_idx,:), blocklist(item_idx).rotation);

            if (new_block(1) < 1 || new_block(2) < 1 || ...
                new_block(1) > grid_size(1) || new_block(2) > grid_size(2) || ...
                placement_matrix(new_block(1), new_block(2)) ~= 0)
                % Invalid placement
                placement_matrix = [];
                return;
            end

            placement_matrix(new_block(1), new_block(2)) = item_idx;
        end
    end
end

function score = countValidConnections(blocklist, placement_matrix)
    score = 0;
    for item_idx = 1:length(blocklist)
        % TODO: Check if types are compatible for connection
        % TODO: Certain items can connect anywhere 
        for connection_block_idx = 1:size(blocklist(item_idx).connection_blocks,1)
            coords = blocklist(item_idx).position + ...
                rotateCoords(blocklist(item_idx).connection_blocks(connection_block_idx,:), blocklist(item_idx).rotation);

            if (coords(1) < 1 || coords(2) < 1 || ...
                coords(1) > size(placement_matrix,1) || coords(2) > size(placement_matrix,1))
                continue;
            end

            if placement_matrix(coords(1), coords(2)) > 0
                score = score + 1;
            end
        end
    end
end

function new_coords = rotateCoords(position, rotation)
    % 1=90degrees right 2=180degrees right etc
    rad = (-1)*double(rotation)*pi/2;
    rotation_matrix = [cos(rad) -sin(rad); sin(rad) cos(rad)];
    new_coords = transpose(int32(rotation_matrix * transpose(double(position))));
end