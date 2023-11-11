classdef item
    properties 
        position int32
        % 1x2 integer array = coordinates 
        rotation int32
        % 1=90degrees 2=180degrees etc
        blocks int32
        % 2xN array of coordinates relative to origin
        connections_blocks int32
        % 2xN array of coordinates relative to origin
        connections_directions int32
        % 1xN array of rotations for each connections block
        value double
        % Value of the block for the objective function
        connection_modifier double
        % Modifies the value of a block placed at the connection
    end
end