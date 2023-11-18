function main()
    % Import block definitions
    types = block_types();
    blocklist = [types.simple_block types.double_block types.corner_block types.square_block];

    
    % Random placement with abort criterium 
    rng(0,'twister');
    % abortCounter = 0;
    % abortScore = 0;
    % best_configuration = [];
    while 1
        for item_idx = 1:length(blocklist)
            blocklist(item_idx).position = randi([0 6],1,2);
            blocklist(item_idx).rotation = randi([0 3],1,1);
        end

        placement_matrix = generatePlacementMatrix(blocklist);

        if ~isempty(placement_matrix); break; end;
        
        % score = objectiveFunction(blocklist);
        % if score > abortScore
        %     abortScore = score;
        %     abortCounter = 0;
        %     best_configuration = blocklist;
        %     disp("Current Best Score:");
        %     disp(score);
        % else
        %     abortCounter = abortCounter + 1;
        % end
    
        % if abortCounter > 1e4; break; end
    end
end

function placement_matrix = generatePlacementMatrix(blocklist)
    placement_matrix = zeros(10);
    for item_idx = 1:length(blocklist)
        for block_idx = 1:size(blocklist(item_idx).blocks,1)
            new_block = blocklist(item_idx).position + rotateCoords(blocklist(item_idx).blocks(block_idx,:), blocklist(item_idx).rotation);
            if new_block(1) < 1 | new_block(2) < 1 | placement_matrix(new_block(1), new_block(2)) ~= 0 
                % Invalid placement
                placement_matrix = [];
                return;
            end
            placement_matrix(new_block(1), new_block(2)) = item_idx;
        end
    end
end

function new_coords = rotateCoords(position, rotation)
    % 1=90degrees right 2=180degrees right etc
    rad = (-1)*double(rotation)*pi/2;
    rotation_matrix = [cos(rad) -sin(rad); sin(rad) cos(rad)];
    new_coords = transpose(int32(rotation_matrix * transpose(double(position))));
end