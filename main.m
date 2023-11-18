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
            blocklist(item_idx).position = randi([0 6],1,2);
            blocklist(item_idx).rotation = randi([0 3],1,1);
        end

        placement_matrix = generatePlacementMatrix(blocklist);

        if isempty(placement_matrix); continue; end;

        score = countValidConnections(blocklist, placement_matrix);
        if score > abortScore
            abortScore = score;
            abortCounter = 0;
            best_configuration = placement_matrix;
            disp("Current Best Score:");
            disp(score);
        else
            abortCounter = abortCounter + 1;
        end
    
        if abortCounter > 1e4; break; end
    end
    heatmap(best_configuration);
end

function placement_matrix = generatePlacementMatrix(blocklist)
    bag_size = 10;
    placement_matrix = zeros(bag_size);

    for item_idx = 1:length(blocklist)
        for block_idx = 1:size(blocklist(item_idx).blocks,1)

            new_block = blocklist(item_idx).position + ...
                rotateCoords(blocklist(item_idx).blocks(block_idx,:), blocklist(item_idx).rotation);

            if (new_block(1) < 1 || new_block(2) < 1 || ...
                new_block(1) > bag_size || new_block(2) > bag_size || ...
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