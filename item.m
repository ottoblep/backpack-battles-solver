% Defines the properties of an item consisting of blocks
classdef item
    properties 
        position int32
        % 1x2 integer array = coordinates 
        rotation int32
        % 1=90degrees 2=180degrees etc
        blocks int32
        % 2xN array of coordinates relative to origin
        connection_blocks int32
        % 2xN array of coordinates affected by the block 
        value double
        % Value of the block for the objective function
        connection_modifier double
        % Modifies the value of a block placed at the connection
    end
end