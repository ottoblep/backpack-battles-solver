% Defines the properties of an item consisting of blocks
classdef item
    properties 
        position int32
        % 1x2 integer array = coordinates 
        rotation int32
        % 1=90degrees 2=180degrees etc
        blocks int32
        % 2xN array of relative coordinates relative to origin
        connection_blocks int32
        % 2xN array of relative coordinates that can connect the block 
        connection_type
        % Type(s) of item that can connect the block
        item_type
        % Decides which other items can be modified by this item
    end
end