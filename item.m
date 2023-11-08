classdef item
    properties 
        position int32
        % 1x2 integer array = coordinates 
        rotation int32
        % 0=right 1=down 2=left 3=up
        blocks int32
        % 2xN array of coordinates relative to origin
        connections_blocks int32
        % 2xN array of coordinates relative to origin
        connections_directions int32
        % 1xN array of rotations for each connections block
    end
end