simple_block = item();
simple_block.blocks = [0 0];
simple_block.connections_blocks = [0 0];
simple_block.connections_directions = [0];

double_block = item();
double_block.blocks = [0 0; 0 1];
double_block.connections_blocks = [0 0];
double_block.connections_directions = [0];


blocklist = [simple_block double_block];

% Constructive placement

% Place item with most connections in the middle
blocklist(findMaxConnections(blocklist)).position = [0 0];
blocklist(findMaxConnections(blocklist)).rotation = 0;

% Place second block next to it
blocklist(2).position = [0 1];
blocklist(2).rotation = [1];

disp(checkBuildValidity(blocklist));

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
            new_block = rotateBlock(blocklist(i).position + blocklist(i).blocks(k,:), blocklist(i).rotation);
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
    % 1=90degrees 2=180degrees etc
    rad = rotation*pi/2;
    rotation_matrix = [cos(rad) -sin(rad); sin(rad) cos(rad)];
    new_coords = rotation_matrix * position;
end