simple_block = item();
simple_block.blocks = [0 0];
simple_block.connections_blocks = [];
simple_block.connections_directions = [];
simple_block.value = 1;
simple_block.connection_modifier = 1;
checkBlockIntegrity(simple_block);

double_block = item();
double_block.blocks = [0 0; 0 1];
double_block.connections_blocks = [0 0; 0 1];
double_block.connections_directions = [3 1];
double_block.value = 1;
double_block.connection_modifier = 2;
checkBlockIntegrity(double_block);

corner_block = item();
corner_block.blocks = [0 0; 0 1; 1 1];
corner_block.connections_blocks = [0 0; 1 1];
corner_block.connections_directions = [1 0];
corner_block.value = 1;
corner_block.connection_modifier = 3;
checkBlockIntegrity(corner_block);

blocklist = [simple_block double_block corner_block];

% Constructive placement

% Test placement
blocklist(1).position = [0 0];
blocklist(1).rotation = 0;
blocklist(2).position = [0 1];
blocklist(2).rotation = 1;
blocklist(3).position = [-1 0];
blocklist(3).rotation = 2;

disp(checkBuildValidity(blocklist));
drawBag(blocklist);

function mostConnsIndex = findMaxConnections(blocklist)
    maxConns = 0;
    mostConnsIndex = 0;
    for i = 1:length(blocklist)
        numConns = length(blocklist(i).connections_directions);
        if  numConns > maxConns
            maxConns = numConns;
            mostConnsIndex = i;
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

function checkBlockIntegrity(block)
    assert(size(block.connections_blocks)<size(block.blocks));
    assert(size(block.connections_blocks,1)==size(block.connection_directions));
end